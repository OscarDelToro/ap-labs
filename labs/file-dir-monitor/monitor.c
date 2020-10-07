#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>
#include <sys/inotify.h>
#include <string.h>
#include<signal.h>

#define BUFFERSIZE (100 *(sizeof(struct inotify_event) + 1000))
struct sigaction old_action;
int notiRef, wd;
struct descriptor {
    char name[1000];
    int wd;
};
struct descriptor wd_structs[(100 * (sizeof(struct descriptor) + 1000))];
int wd_index = 0;
int wd_size = 0;

void sigint_handler(int sig_no)
{
    warnf("Monitor terminated\n");
    sigaction(SIGINT, &old_action, NULL);
    kill(0, SIGINT);
}
void makeStruct(const char * name, int watchDescriptor){
    struct descriptor tmpStruct;
    strcpy(tmpStruct.name, name);
    tmpStruct.wd = wd;
    wd_structs[wd_index++] = tmpStruct;
    wd_size++;
}
static int addWatcher(const char *filePath, const struct stat *sb, int type){
    int watchDescriptor;
    if((watchDescriptor = inotify_add_watch(notiRef, filePath, IN_CREATE | IN_DELETE | IN_MODIFY)) <0){
        errorf("Failed creating watch descriptor");
        exit(6);
    }
    makeStruct(filePath, watchDescriptor);
    return 0;
}
static void toLog(struct inotify_event *i) {
    if (i->mask & IN_CREATE){
        infof("Added %s\n",i->name);
    }        
    else if (i->mask & IN_DELETE){
        infof("Removed %s\n",i->name);
    }       
    else if (i->mask & IN_MOVED_FROM){
        infof("Moved %s ",i->name);
    }   
    else if (i->mask & IN_MOVED_TO){
        infof("to %s\n", i->name);
    }
    else{
        infof("Unknown action in %s\n", i->name);
    }      
}
int main(int argc, char** argv){
    if(argc < 2){
        errorf("Wrong arguments");
        exit(2);
    }
    const char *path= argv[1];
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = &sigint_handler;
    sigaction(SIGINT, &action, &old_action);
    
    char buffer[BUFFERSIZE];
    ssize_t notiDescriptor;
    char *tmp;
    struct inotify_event *event;
    
    notiRef = inotify_init();
    if(notiRef <0){
        errorf("inotify failed");
        exit(3);
    }
    if(ftw(path, addWatcher, 200)<0){
        errorf("Failed exploring the tree");
        exit(4);
    }
    while(1){
        if((notiDescriptor = read(notiRef, buffer, BUFFERSIZE)) <= 0){
            errorf("inotify descriptor failed");
            exit(1);
        }
        for(tmp = buffer; tmp < buffer + notiDescriptor;){
            event = (struct inotify_event * ) tmp;
            toLog(event);
            ftw(path, addWatcher, 200);
            tmp += sizeof(struct inotify_event) + event->len;
        }
    }
    return 0;
}
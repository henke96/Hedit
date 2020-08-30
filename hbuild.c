#include <stdint.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>

struct hbuild_job {
    char *program;
    char **argv;
    char **envp;
};

void hbuild_job_init(struct hbuild_job *self, char *program, char **argv) {
    self->program = program;
    self->argv = argv;
    self->envp = NULL;
}

void hbuild_job_print(struct hbuild_job *self) {
    printf("%s", self->program);
}

// Return: -1=error, 0=false, 1=true
int hbuild_isFileNewer(char *filePath, char *otherPath) {
    struct stat buffer;
    if (stat(filePath, &buffer) != 0) return -1;
    time_t fileTime = buffer.st_mtime;

    if (stat(otherPath, &buffer) != 0) return -1;
    return fileTime > buffer.st_mtime;
}

int hbuild_run(struct hbuild_job *jobQueue, uint32_t jobQueueLen, uint32_t parallelJobs) {
    if (parallelJobs > jobQueueLen) parallelJobs = jobQueueLen;

    uint32_t queuePos;
    for (queuePos = 0; queuePos < parallelJobs; ++queuePos) {
        pid_t pid;
        if(posix_spawnp(&pid, jobQueue[queuePos].program, NULL, NULL, jobQueue[queuePos].argv, jobQueue[queuePos].envp) != 0) {
            return -1;
        }
        printf("starting pid: %d (%s)\n", pid, jobQueue[queuePos].argv[1]);
    }

    for (; queuePos < jobQueueLen; ++queuePos) {
        int wstatus;
        pid_t pid = wait(&wstatus);
        if (pid == -1) return 1;
        printf("pid %d finished\n", pid);

        if(posix_spawnp(&pid, jobQueue[queuePos].program, NULL, NULL, jobQueue[queuePos].argv, jobQueue[queuePos].envp) != 0) {
            return -1;
        }
        printf("starting pid: %d (%s)\n", pid, jobQueue[queuePos].argv[1]);
    }
    pid_t pid;
    while ((pid = wait(NULL)) != -1) {
        printf("pid %d finished\n", pid);
    }
    return 0;
}

int main(int argc, char **argv) {
    int res = hbuild_isFileNewer("test1", "test2");
    if (res < 0) {
        printf("error\n");
        return 1;
    }
    if (res) {
        printf("newer\n");
    } else {
        printf("not newer\n");
    }

    char *argv2[] = {
        "sleep",
        "5",
        NULL
    };
    char *argv3[] = {
        "sleep",
        "2",
        NULL
    };
    struct hbuild_job job;
    struct hbuild_job job2;
    hbuild_job_init(&job, "sleep", argv2);
    hbuild_job_init(&job2, "sleep", argv3);

    struct hbuild_job jobs[] = {
        job, job2, job2, job, job, job2, job, job, job2, job
    };
    hbuild_run(jobs, 10, 3);
    return 0;
}
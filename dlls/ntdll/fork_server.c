

// this file handles the creation of the forkserver, which creates
// the copiable environment for running a child process

// maybe not needed? dunno. see forkserver loop in loader.c

/**
 * GENERAL OUTLINE
 */

void wine_fork_server_loop(void) {
    while(1) {
        read_fuzzer_input();
        pid = fork();
        if (pid == 0) { /* Child */
            setup_crash_handling();
            execute_input();
            send_results();
            _exit(0);
        }
        /* Parent */
        monitor_child(pid);
    }
}

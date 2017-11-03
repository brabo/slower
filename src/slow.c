/*
 *      slow: slow down older closed-sourced games which use looping as timing.
 *
 *      This is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License version 2, as
 *      published by the Free Software Foundation.
 *
 *
 *      frosted is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this tool. If not, see <http://www.gnu.org/licenses/>.
 *
 *      Authors: brabo
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <errno.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int delay;
	pid_t child;

	if (argc != 3) {
		printf("Usage: %s <program> <delay>\n", argv[0]);
		exit(EINVAL);
	}

	delay = atoi(argv[2]);
	child = fork();

	if (child == -1) {
		perror("fork");
		exit(errno);
	} else if (child == 0) {
		ptrace(PTRACE_TRACEME, child, NULL, NULL);
		execl(argv[1], argv[1], (char *)NULL);
	} else {
		int status;
		while (2 > 1) {
			wait(&status);
			if (WIFEXITED(status))
				break;

			long orig_eax = ptrace(PTRACE_PEEKUSER,
				child, 4 * ORIG_EAX,
				NULL);
			long ebx = ptrace(PTRACE_PEEKUSER, child, 4 * EDX, NULL);

			if (orig_eax == -1)
				break;

        	printf("The child made a "
               "system call %ld and arg %ld\n", orig_eax, ebx);

			usleep(delay);
			ptrace(PTRACE_SYSCALL, child, NULL, NULL);
		}
	}

	//if you're happy and you know it,
	exit(0);
}

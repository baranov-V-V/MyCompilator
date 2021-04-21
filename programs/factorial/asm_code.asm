section .text

global _start

_start:        
call main      
mov rax, 0x3C  
xor rdi, rdi   
syscall        
             

main:            
push rbp       
mov rbp, rsp   
sub rsp, 8 * 2
push 0
pop rax   
mov [rbp - 8 * 1], rax ; assembling declaration var [num]
call GetNumber          
mov [rbp - 8 * 1], rax 
mov rax, [rbp - 8 * 1] ; var [num]
push rax  
call factorial
push rax ; return value of called function
pop rax   
mov [rbp - 8 * 2], rax ; assembling declaration var [result]
mov rax, [rbp - 8 * 2] ; var [result]
push rax  
pop rdi        
call PrintSign 
add rsp, 8 * 2 
pop rbp         
ret 8 * 0  ; returning from function [main]
             

factorial:            
push rbp       
mov rbp, rsp   
sub rsp, 8 * 1
mov rax, [rbp + 8 * 1 + 8]   
mov [rbp - 8 * 1], rax; fetching arg [num] into local stack frame
mov rax, [rbp - 8 * 1] ; var [num]
push rax  
push 0

;begin assembling <compare> with label [0]
pop rbx      
pop rax      
cmp rax, rbx 
je true_0           
push 0            
jmp end_comp_0   
true_0:          
push 1            
jmp end_comp_0   
end_comp_0:      

;end compiling <compare> with label [0]

;begin assembling <if> with label [1]
pop rax      
cmp rax, 1   
je  true_1  
jmp false_1 

true_1:     
push 1
pop rax ; getting return value
add rsp, 8 * 1 
pop rbp         
ret 8 * 1  ; returning from function [factorial]
jmp end_cond_1  

false_1:        
mov rax, [rbp - 8 * 1] ; var [num]
push rax  
mov rax, [rbp - 8 * 1] ; var [num]
push rax  
push 1

pop rbx  ;begin assembling math op
pop rax  
sub rax, rbx
push rax  ;end assembling math op
call factorial
push rax ; return value of called function

pop rbx  ;begin assembling math op
pop rax  
xor rdx, rdx 
mul rbx      
push rax  ;end assembling math op
pop rax ; getting return value
add rsp, 8 * 1 
pop rbp         
ret 8 * 1  ; returning from function [factorial]
end_cond_1:     

;end assembling <if> with label [1]
PrintSign:													 
				 xor r14, r14							   
																	   
        mov rax, mask        		   
																	   
        and rax, rdi							   
        or rax, rax							   
        jz print_num           	   
     														   
        neg rdi									   
																	   
        mov al, '-'							   
        mov [Buffer + r14], al		   
        inc r14       						   
																	   
    print_num:									     
        mov rsi, 10						     
        call PrintNum					     
																     
        ret										     
;END PrintSign          			  	   
																	   
PrintNum:         								   
        mov rax, rdi;						   
																	   
        xor rdi, rdi							   
        xor rcx, rcx							   
																	   
loop_number_transl:							   
        xor rdx, rdx							   
																	   
        div rsi									   
																	   
        mov rbx, rdx							   
        add rbx, Numbers					   
        mov cl, byte [rbx]				   
        push rcx									   
																	   
        inc rdi									   
        or rax, rax							   
        jnz loop_number_transl		   
																	   
loop_number_print:								   
        pop rax									   
																	   
        mov byte [Buffer + r14], al 
        inc r14									   
																	   
        dec rdi									   
        or rdi, rdi							   
        jne loop_number_print		   
																	   
        call PrintBuff						   
        ret											   
;END PrintDec										   
																	   
PrintBuff:												   
        or r14, r14					       
        jz end_print_buff		       
																		 
				 mov al, 0xA								 
				 mov byte [Buffer + r14], al 
				 inc r14				  		       
																	   
        mov rax, 1						       
        mov rsi, Buffer             
        mov rdx, r14                
        mov rdi, 1						       
        syscall							       
                                    
        xor r14, r14					       
                                    
end_print_buff:							       
        ret									       
;END PrintBuff								       

GetNumber:													 
        xor r10, r10								 
        mov rax, 0						  		 
	       mov rdi, 1									 
	       mov rsi, Buffer						 
	       mov rdx, buf_size					 
	       syscall	           				 
	       													   
	       xor r14, r14 							 
	       xor rax, rax								 
        														 
	       mov byte al, [Buffer + r14] 
	       cmp al, '-'								 
	       jne no_neg									 
	       inc r14								     
	       mov r10, 1							     
																		 
    no_neg:												 
				 xor rdi, rdi								 
        														 
        mov byte al, [Buffer + r14] 
        inc r14										 
																		 
    loop_number_get:								 
																     
	       sub al, '0'							   
	       														 
	       add rdi, rax								 
	       														 
	       mov byte al, [Buffer + r14] 
	       inc r14										 
              											 
	       cmp al, 0xA								 
	       je end_get_number					 
	       														 
	       mov bl, al								   
        														 
	       mov rax, rdi								 
	       mov rdi, 10								 
	       														 
	       mul rdi										 
	       mov rdi, rax								 
              											 
	       xor rax, rax								 
	       mov al, bl									 
	       jmp loop_number_get				 
																		 
    end_get_number:								 
																		 
	       mov rax, rdi								 
              											 
	       cmp r10, 1									 
	       jne end_function						 
        														 
	       neg rax										 
																		 
    end_function:			 						 
 																	 
        ret												 

section    .data								     
Numbers:		db "0123456789ABCDEF"  
buf_size   equ 64								   
mask       equ 8000000000000000h    
Buffer:    times buf_size db 0	     

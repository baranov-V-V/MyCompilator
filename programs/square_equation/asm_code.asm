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
sub rsp, 8 * 3
push 0
pop rax   
mov [rbp - 8 * 1], rax ; assembling declaration var [a]
push 0
pop rax   
mov [rbp - 8 * 2], rax ; assembling declaration var [b]
push 0
pop rax   
mov [rbp - 8 * 3], rax ; assembling declaration var [c]
call GetNumber          
mov [rbp - 8 * 1], rax 
call GetNumber          
mov [rbp - 8 * 2], rax 
call GetNumber          
mov [rbp - 8 * 3], rax 
mov rax, [rbp - 8 * 1] ; var [a]
push rax  
push 0

;begin assembling <compare> with label [0]
pop rbx      
pop rax      
cmp rax, rbx 
jne true_0           
push 0            
jmp end_comp_0   
true_0:          
push 1            
jmp end_comp_0   
end_comp_0:      

;begin assembling <if> with label [1]
pop rax      
cmp rax, 1   
je  true_1  
jmp false_1 

true_1:     
mov rax, [rbp - 8 * 3] ; var [c]
push rax  
mov rax, [rbp - 8 * 2] ; var [b]
push rax  
mov rax, [rbp - 8 * 1] ; var [a]
push rax  
call Solvesquare
add rsp, 8 * 3
jmp end_cond_1  

false_1:        
mov rax, [rbp - 8 * 3] ; var [c]
push rax  
mov rax, [rbp - 8 * 2] ; var [b]
push rax  
call Solvelinear
add rsp, 8 * 2
end_cond_1:     

;end assembling <if> with label [1]
add rsp, 8 * 3 
pop rbp         
ret ; returning from function [main]
             

Solvelinear:            
push rbp       
mov rbp, rsp   
sub rsp, 8 * 2
mov rax, [rbp + 8 * 1 + 8]   
mov [rbp - 8 * 1], rax; fetching arg [k] into local stack frame
mov rax, [rbp + 8 * 2 + 8]   
mov [rbp - 8 * 2], rax; fetching arg [b] into local stack frame
mov rax, [rbp - 8 * 1] ; var [k]
push rax  
push 0

;begin assembling <compare> with label [2]
pop rbx      
pop rax      
cmp rax, rbx 
je true_2           
push 0            
jmp end_comp_2   
true_2:          
push 1            
jmp end_comp_2   
end_comp_2:      

;begin assembling <if> with label [3]
pop rax      
cmp rax, 1   
je  true_3  
jmp false_3 

true_3:     
mov rax, [rbp - 8 * 2] ; var [b]
push rax  
push 0

;begin assembling <compare> with label [4]
pop rbx      
pop rax      
cmp rax, rbx 
je true_4           
push 0            
jmp end_comp_4   
true_4:          
push 1            
jmp end_comp_4   
end_comp_4:      

;begin assembling <if> with label [5]
pop rax      
cmp rax, 1   
je  true_5  
jmp false_5 

true_5:     
push 100500
pop rdi        
call PrintSign 
jmp end_cond_5  

false_5:        
push 0
pop rdi        
call PrintSign 
end_cond_5:     

;end assembling <if> with label [5]
jmp end_cond_3  

false_3:        
push 1
pop rdi        
call PrintSign 
push 0
mov rax, [rbp - 8 * 2] ; var [b]
push rax  

pop rbx  ;begin assembling math op [while]
pop rax  
sub rax, rbx
push rax  ;end assembling math op
mov rax, [rbp - 8 * 1] ; var [k]
push rax  

pop rbx  ;begin assembling math op [if]
pop rax  
cqo           
idiv rbx      
push rax  ;end assembling math op
pop rdi        
call PrintSign 
end_cond_3:     

;end assembling <if> with label [3]
add rsp, 8 * 2 
pop rbp         
ret ; returning from function [Solvelinear]
             

Solvesquare:            
push rbp       
mov rbp, rsp   
sub rsp, 8 * 7
mov rax, [rbp + 8 * 1 + 8]   
mov [rbp - 8 * 1], rax; fetching arg [a] into local stack frame
mov rax, [rbp + 8 * 2 + 8]   
mov [rbp - 8 * 2], rax; fetching arg [b] into local stack frame
mov rax, [rbp + 8 * 3 + 8]   
mov [rbp - 8 * 3], rax; fetching arg [c] into local stack frame
mov rax, [rbp - 8 * 2] ; var [b]
push rax  
mov rax, [rbp - 8 * 2] ; var [b]
push rax  

pop rbx  ;begin assembling math op [return]
pop rax  
cqo           
imul rbx      
push rax  ;end assembling math op
push 4
mov rax, [rbp - 8 * 1] ; var [a]
push rax  

pop rbx  ;begin assembling math op [return]
pop rax  
cqo           
imul rbx      
push rax  ;end assembling math op
mov rax, [rbp - 8 * 3] ; var [c]
push rax  

pop rbx  ;begin assembling math op [return]
pop rax  
cqo           
imul rbx      
push rax  ;end assembling math op

pop rbx  ;begin assembling math op [while]
pop rax  
sub rax, rbx
push rax  ;end assembling math op
pop rax   
mov [rbp - 8 * 4], rax ; assembling declaration var [desc]
push 0
pop rax   
mov [rbp - 8 * 5], rax ; assembling declaration var [x1]
push 0
pop rax   
mov [rbp - 8 * 6], rax ; assembling declaration var [x2]
mov rax, [rbp - 8 * 4] ; var [desc]
push rax  
push 0

;begin assembling <compare> with label [6]
pop rbx      
pop rax      
cmp rax, rbx 
jg true_6           
push 0            
jmp end_comp_6   
true_6:          
push 1            
jmp end_comp_6   
end_comp_6:      

;begin assembling <if> with label [7]
pop rax      
cmp rax, 1   
je  true_7  
jmp false_7 

true_7:     
mov rax, [rbp - 8 * 4] ; var [desc]
push rax  
pop rax ; assembling sqrt    
cvtsi2sd xmm0, rax           
sqrtsd xmm0, xmm0            
cvttsd2si rax, xmm0          
push rax                     
pop rax   
mov [rbp - 8 * 7], rax ; assembling declaration var [root]
push 0
mov rax, [rbp - 8 * 2] ; var [b]
push rax  

pop rbx  ;begin assembling math op [while]
pop rax  
sub rax, rbx
push rax  ;end assembling math op
mov rax, [rbp - 8 * 7] ; var [root]
push rax  

pop rbx  ;begin assembling math op [while]
pop rax  
sub rax, rbx
push rax  ;end assembling math op
push 2
mov rax, [rbp - 8 * 1] ; var [a]
push rax  

pop rbx  ;begin assembling math op [return]
pop rax  
cqo           
imul rbx      
push rax  ;end assembling math op

pop rbx  ;begin assembling math op [if]
pop rax  
cqo           
idiv rbx      
push rax  ;end assembling math op
pop rax   
mov [rbp - 8 * 5], rax ;assembling assignment var [x1]
push 0
mov rax, [rbp - 8 * 2] ; var [b]
push rax  

pop rbx  ;begin assembling math op [while]
pop rax  
sub rax, rbx
push rax  ;end assembling math op
mov rax, [rbp - 8 * 7] ; var [root]
push rax  

pop rbx  ;begin assembling math op [double]
pop rax  
add rax, rbx
push rax  ;end assembling math op
push 2
mov rax, [rbp - 8 * 1] ; var [a]
push rax  

pop rbx  ;begin assembling math op [return]
pop rax  
cqo           
imul rbx      
push rax  ;end assembling math op

pop rbx  ;begin assembling math op [if]
pop rax  
cqo           
idiv rbx      
push rax  ;end assembling math op
pop rax   
mov [rbp - 8 * 6], rax ;assembling assignment var [x2]
push 2
pop rdi        
call PrintSign 
mov rax, [rbp - 8 * 5] ; var [x1]
push rax  
pop rdi        
call PrintSign 
mov rax, [rbp - 8 * 6] ; var [x2]
push rax  
pop rdi        
call PrintSign 
jmp end_cond_7  

false_7:        
mov rax, [rbp - 8 * 4] ; var [desc]
push rax  
push 0

;begin assembling <compare> with label [8]
pop rbx      
pop rax      
cmp rax, rbx 
je true_8           
push 0            
jmp end_comp_8   
true_8:          
push 1            
jmp end_comp_8   
end_comp_8:      

;begin assembling <if> with label [9]
pop rax      
cmp rax, 1   
je  true_9  
jmp false_9 

true_9:     
push 0
mov rax, [rbp - 8 * 2] ; var [b]
push rax  

pop rbx  ;begin assembling math op [while]
pop rax  
sub rax, rbx
push rax  ;end assembling math op
push 2
mov rax, [rbp - 8 * 1] ; var [a]
push rax  

pop rbx  ;begin assembling math op [return]
pop rax  
cqo           
imul rbx      
push rax  ;end assembling math op

pop rbx  ;begin assembling math op [if]
pop rax  
cqo           
idiv rbx      
push rax  ;end assembling math op
pop rax   
mov [rbp - 8 * 5], rax ;assembling assignment var [x1]
push 1
pop rdi        
call PrintSign 
mov rax, [rbp - 8 * 5] ; var [x1]
push rax  
pop rdi        
call PrintSign 
jmp end_cond_9  

false_9:        
push 0
pop rdi        
call PrintSign 
end_cond_9:     

;end assembling <if> with label [9]
end_cond_7:     

;end assembling <if> with label [7]
add rsp, 8 * 7 
pop rbp         
ret ; returning from function [Solvesquare]
																		 
;This is standart input output funcs
																		 
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
                                    
;begin of PrintNum 	  				     
																     
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
																	   
;begin of PrintBuff								 
																	   
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
                                    
;END PrintSign								       

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
;END GetNumber                      

section    .data								     
Numbers:		db "0123456789ABCDEF"  
buf_size   equ 64								   
mask       equ 8000000000000000h    
Buffer:    times buf_size db 0	     

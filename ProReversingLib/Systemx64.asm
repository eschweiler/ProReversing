.code 

getPeb proc
	mov rax, qword ptr gs:[60h]
	ret
getPeb endp

end
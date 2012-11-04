.model flat
.code 

assume fs:nothing

_getPeb proc
	mov eax, fs:[30h]
	ret
_getPeb endp

end
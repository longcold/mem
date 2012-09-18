
.386
.MODEL FLAT
PUBLIC	_init	    ; asm에서 만든 함수를 다른 파일에서 init 함수 쓸수 있다.		
EXTRN _test:NEAR32  ; 외부 함수 호출 호출한다
EXTRN _LDST:NEAR32   


.CODE		
_init	PROC	NEAR32	;함수시작
	push	eax	;베이스 포인트자리를 하나 만들어준다 (esp를 위로한칸올림) *엔트리코드
	call	_test
	call	_LDST
	
_init	ENDP			
END	









.386
.MODEL FLAT
PUBLIC	_init	    ; asm���� ���� �Լ��� �ٸ� ���Ͽ��� init �Լ� ���� �ִ�.		
EXTRN _test:NEAR32  ; �ܺ� �Լ� ȣ�� ȣ���Ѵ�
EXTRN _LDST:NEAR32   


.CODE		
_init	PROC	NEAR32	;�Լ�����
	push	eax	;���̽� ����Ʈ�ڸ��� �ϳ� ������ش� (esp�� ������ĭ�ø�) *��Ʈ���ڵ�
	call	_test
	call	_LDST
	
_init	ENDP			
END	








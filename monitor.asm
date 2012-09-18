
.386
.MODEL FLAT

PUBLIC	_STST			;C���� ���� ���
PUBLIC	_MD		
PUBLIC	_LDST		

.CODE					; �ڵ����
;���� cpu ���� ����ü ����
_STST	PROC	NEAR32	;�Լ�����
	push	ebp			;���̽� ����Ʈ�ڸ��� �ϳ� ������ش� (esp�� ������ĭ�ø�) *��Ʈ���ڵ�
	mov	ebp,esp			;esp ��ġ�� ebp ���� �ִ´�	*��Ʈ���ڵ�

	pushfd
	mov	esp, [ebp + 8]		;esp�� c8����ü�� �ּҸ� ����Ų��
	add	esp, 40			;����ü �ּҷκ��� 40 �Ʒ��� ����
	pushad
	push	[ebp + 4]		;����ġ�� �����ּҸ� Ǫ���Ѵ� (���� esp ��ġ����� ���� ������ ���δ�)	
	push	[ebp - 4]		;�� ��ġ�� elf���� Ǫ���Ѵ�

	add	esp, 24
	mov	eax, ebp
	add	eax,12
	push	eax
	push	[ebp]
	
	mov	esp,ebp			; esp �� ebp �ּҸ� �༭ ������ ������
	pop	ebp				; ebp�� ����
	ret					; �Լ� ����	pop EIP
	
_STST	ENDP			


;------cpu�޸𸮰� ����ü�� ����----------------------
_MD	PROC	NEAR32 
	push	ebp
	mov	ebp, esp	

	push	ebx
	mov	ebx, [ebp + 8]
	mov	eax, 0
	mov	al, [ebx] 
	pop	ebx

	mov	esp, ebp	
	pop	ebp	
	ret
_MD	ENDP


;cputemp    eax: &cpuinfo,  eip:�ڵ��ּ�,  esp:�����ּ� cpu�� ����
_LDST	PROC	NEAR32
	mov	esp, [esp + 4] ; esp = context -> efl
	popfd		       ; efl = esp
	
	pop	eax		; eax = context -> eip
	mov	ebx, [esp + 12] ; ebx = main_esp
	mov	[ebx - 4], eax	; esp - 4 = r.a // r.a = context -> eip ;ebp���� 

	popad			; 
	mov	esp,	[esp-20]; (Context -> esp�� pop �ȵ�) esp = Context -> esp ;esp���� 	
	sub	esp,	4	; esp = r.a ���� ���Խ��Ѿ� �Ѵ�		   ;esp����
	
	ret			; pop �ȴ�, r.a�� ����, esp = [esp + 4]
_LDST	ENDP

END	








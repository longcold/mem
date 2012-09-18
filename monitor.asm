
.386
.MODEL FLAT

PUBLIC	_STST			;C에서 접근 허용
PUBLIC	_MD		
PUBLIC	_LDST		

.CODE					; 코드시작
;복귀 cpu 정보 구조체 삽입
_STST	PROC	NEAR32	;함수시작
	push	ebp			;베이스 포인트자리를 하나 만들어준다 (esp를 위로한칸올림) *엔트리코드
	mov	ebp,esp			;esp 위치에 ebp 값을 넣는다	*엔트리코드

	pushfd
	mov	esp, [ebp + 8]		;esp가 c8구조체의 주소를 가르킨다
	add	esp, 40			;구조체 주소로부터 40 아래에 간다
	pushad
	push	[ebp + 4]		;이위치에 리턴주소를 푸쉬한다 (현재 esp 위치루부터 위로 스택이 쌓인다)	
	push	[ebp - 4]		;이 위치에 elf값을 푸쉬한다

	add	esp, 24
	mov	eax, ebp
	add	eax,12
	push	eax
	push	[ebp]
	
	mov	esp,ebp			; esp 에 ebp 주소를 줘서 밑으로 내린다
	pop	ebp				; ebp를 복구
	ret					; 함수 리턴	pop EIP
	
_STST	ENDP			


;------cpu메모리값 구조체에 삽입----------------------
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


;cputemp    eax: &cpuinfo,  eip:코드주소,  esp:스택주소 cpu로 복귀
_LDST	PROC	NEAR32
	mov	esp, [esp + 4] ; esp = context -> efl
	popfd		       ; efl = esp
	
	pop	eax		; eax = context -> eip
	mov	ebx, [esp + 12] ; ebx = main_esp
	mov	[ebx - 4], eax	; esp - 4 = r.a // r.a = context -> eip ;ebp셋팅 

	popad			; 
	mov	esp,	[esp-20]; (Context -> esp는 pop 안됨) esp = Context -> esp ;esp셋팅 	
	sub	esp,	4	; esp = r.a 까지 포함시켜야 한다		   ;esp셋팅
	
	ret			; pop 된다, r.a로 복귀, esp = [esp + 4]
_LDST	ENDP

END	








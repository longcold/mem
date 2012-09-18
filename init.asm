.386
.MODEL  FLAT
        
PUBLIC  _init                    ;외부에서 접근이 가능하게 한다. 이때 Under Bar를 붙여줘야 외부에서 볼 수가 있다.

EXTRN   _test:NEAR32
EXTRN   _LDST:NEAR32
        
.CODE                           ;CODE영역 알림.
_init   PROC    NEAR32          ;Procedure 의 시작이라는 것을 알리고, NEAR32은 그냥 일반 주소 체계를 쓰겠다는 말.
        ;; Entry Point

        push    eax
        
        call    _test           ;가상머신 안에서의 엔트리 함수
        call    _LDST
                
_init    ENDP                    ;함수의 끝을 알림.
END                            

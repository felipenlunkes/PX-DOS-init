;;/*********************************************************************/
;;/*                                                                   */
;;/*                                                                   */
;;/*                                                                   */
;;/*                                                                   */
;;/*   #$$%%@!#$%                                                      */
;;/*   !!@#!$!$!$         Sistema Operacional PX-DOS                   */
;;/*   !@@#   #$%                                                      */
;;/*   #$$%   &*$                                                      */
;;/*   $#%@   @#&                                                      */
;;/*   #%$&*(@*@&                                                      */
;;/*   @#$@$#@$%$       2013-2022 (c) Felipe Miguel Nery Lunkes        */
;;/*   $%&*                Todos os direitos reservados                */
;;/*   @#&*                                                            */
;;/*   @&*%       Esse software se baseia em códigos disponíveis       */
;;/*   #&*@                     em domínio público                     */
;;/*                                                                   */
;;/*                                                                   */
;;/*********************************************************************/
;;/*
;;
;; Copyright (c) 2013-2022, Felipe Miguel Nery Lunkes
;; All rights reserved.
;;
;; Redistribution and use in source and binary forms, with or without
;; modification, are permitted provided that the following conditions are met:
;;
;; * Redistributions of source code must retain the above copyright notice, this
;;   list of conditions and the following disclaimer.
;;
;; * Redistributions in binary form must reproduce the above copyright notice,
;;   this list of conditions and the following disclaimer in the documentation
;;   and/or other materials provided with the distribution.
;;
;; * Neither the name of the copyright holder nor the names of its
;;   contributors may be used to endorse or promote products derived from
;;   this software without specific prior written permission.
;;
;; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;; SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;; CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;; OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;; OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;; */

;; Implementação para a chamada PX-DOS em Assembly

% .model memodel, c 

assume cs:_CODIGO, ds:DGROUP

_DADOS   segment word public 'DATA'
_DADOS   ends

_BSS    segment word public 'BSS'
_BSS    ends

_CODIGO segment word public 'CODE' ;; Início da área de Código (Texto)


public __execdriver ;; Declara público esta função, que será chamada pelo módulo em C

__execdriver proc ;; Função que irá pedir ao Kernel o carregamento do Driver

push bp

mov bp, sp

push ds
push dx
push es
push bx
push cx
push si
push di

mov dx, [bp + 6]
mov ds, [bp + 8]
mov bx, [bp + 10]
mov es, [bp + 12]

jmp short chamar_Kernel_PXDOS

salvar_sp dw ? ;; Aqui será salvo o valor de SP antes da chamada
salvar_ss dw ? ;; Aqui será salvo o valor de SS antes da chamada

chamar_Kernel_PXDOS: ;; Irá adequar os parâmetros recebidos e realizar a chamada

mov salvar_sp, sp ;; Salvar SP
mov salvar_ss, ss ;; Salvar SS

mov al, 0
mov ah, 07h
cld              ;; Para evitar BUGS com o Kernel
int 90h          ;; Chama o Kernel do PX-DOS

cli ;; Desabilita as interrupções

mov ss, salvar_ss ;; Restaurar o valor inicial de SS, antes da chamada ao Kernel
mov sp, salvar_sp ;; Restaurar o valor inicial de SP, antes da chamada ao Kernel

sti ;; Habilita as interrupções

;; Restaurar o estado da máquina

pop di 
pop si
pop cx
pop bx
pop es
pop dx
pop ds
pop bp

ret ;; Retorna ao Módulo em C

__execdriver endp ;; Fim da função

_CODIGO ends ;; Fim da área de Código (Texto)

end ; Fim do Módulo
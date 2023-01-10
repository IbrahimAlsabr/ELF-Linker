   .data
debutTAB: .skip 5*4 @ reservation de 20 octets
                @ sans valeur initiale

   .text
   .global main
main:
   ldr r0, ptr_debutTAB @ r0 <- adresse de la première valeur du tableau 
   mov r1, #11 @ r1 <- 11
   str r1, [r0] @ mem[r0] <- r1

   mov r1, #22 @ r1 <- 22
   add r0, r0, #4 @ ajouter 4 à l'adresse précédente parce qu'on est en 32 bits
   str r1, [r0] @ mem[r0] <- r1
   
   mov r1, #33 @ r1 <- 33
   add r0, r0, #4 @ ajouter 4 à l'adresse précédente parce qu'on est en 32 bits
   str r1, [r0] @ mem[r0] <- r1

   mov r1, #44  @ r1 <- 44
   add r0, r0, #4 @ ajouter 4 à l'adresse précédente parce qu'on est en 32 bits
   str r1, [r0] @ mem[r0] <- r1
   
   mov r1, #55 @ r1 <- 55
   add r0, r0, #4 @ ajouter 4 à l'adresse précédente parce qu'on est en 32 bits
   str r1, [r0] @ mem[r0] <- r1

fin:  BX LR

ptr_debutTAB : .word debutTAB


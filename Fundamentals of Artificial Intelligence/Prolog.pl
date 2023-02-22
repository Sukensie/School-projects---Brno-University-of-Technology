% Zad�n� �. 32:
% Napi�te program �e��c� �kol dan� predik�tem u32(LIN1,LIN2), kde LIN1 a LIN2 
% jsou vstupn� ��seln� seznamy. Predik�t je pravdiv� (m� hodnotu true), pokud 
% po�et nulov�ch ��sel seznamu LIN1 je v�t�� ne� po�et nulov�ch ��sel seznamu 
% LIN2, jinak je predik�t nepravdiv� (m� hodnotu false).

% Testovac� predik�ty: 	                               	
u32_1:- u32([5.1,0,-18,0,0,-13],[0,2.2,-9.47,-13]).		% true
u32_2:- u32([5.1,2,-18,0,1,-13],[0,2.2,-9.47,-13]).		% false
u32_3:- u32([5,2,-18,3,0,-13],[0,2,-9,-13,0]).			% false
u32_r:- write('Zadej LIN1: '),read(LIN1),
	write('Zadej LIN2: '),read(LIN2),
	u32(LIN1,LIN2).
    

u32(LIN1, LIN2):-
    X = 0,
    aggregate_all(count, member(X, LIN1), Count),	%count je typ prov�d�n� built-in operace, member zji��uje, jestli se vyskytuje X v LIN, Count je prom�nn� s v�sledkem
    aggregate_all(count, member(X, LIN2), Count2),
    ( Count > Count2 ->  true ; false ).
    

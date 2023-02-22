% Zadání è. 32:
% Napište program øešící úkol danı predikátem u32(LIN1,LIN2), kde LIN1 a LIN2 
% jsou vstupní èíselné seznamy. Predikát je pravdivı (má hodnotu true), pokud 
% poèet nulovıch èísel seznamu LIN1 je vìtší ne poèet nulovıch èísel seznamu 
% LIN2, jinak je predikát nepravdivı (má hodnotu false).

% Testovací predikáty: 	                               	
u32_1:- u32([5.1,0,-18,0,0,-13],[0,2.2,-9.47,-13]).		% true
u32_2:- u32([5.1,2,-18,0,1,-13],[0,2.2,-9.47,-13]).		% false
u32_3:- u32([5,2,-18,3,0,-13],[0,2,-9,-13,0]).			% false
u32_r:- write('Zadej LIN1: '),read(LIN1),
	write('Zadej LIN2: '),read(LIN2),
	u32(LIN1,LIN2).
    

u32(LIN1, LIN2):-
    X = 0,
    aggregate_all(count, member(X, LIN1), Count),	%count je typ provádìné built-in operace, member zjišuje, jestli se vyskytuje X v LIN, Count je promìnná s vısledkem
    aggregate_all(count, member(X, LIN2), Count2),
    ( Count > Count2 ->  true ; false ).
    

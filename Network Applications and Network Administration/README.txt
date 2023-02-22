/****************************************/
/*                                      */
/*        Tomáš Souček, xsouce15        */
/*                                      */
/****************************************/
		10.11.2022


Sender
-----------
Klientská část aplikace odesílající data
Spuštění: dns_sender [-u UPSTREAM_DNS_IP] {BASE_HOST} {DST_FILEPATH} [SRC_FILEPATH]



Receiver
-----------
Serverová část aplikace naslouchající na DNS portu
Spuštění: dns_receiver {BASE_HOST} {DST_DIRPATH}



Seznam odevzdaných souborů
-----------------------------
dns_sender.c
dns_receiver.c
library.h
Makefile
manual.pdf


Omezení
----------
Projekt byl realizován pomocí protokolu UDP, takže se může stát, že při přenosu většího množství dat, dojde k přetečení UDP paketu
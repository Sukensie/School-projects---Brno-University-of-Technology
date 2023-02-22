Spuštění
./ipk-sniffer [-i rozhraní | --interface rozhraní] {-p ¬¬port} {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}
kde
•	-i eth0 (právě jedno rozhraní, na kterém se bude poslouchat. Nebude-li tento parametr uveden, či bude-li uvedené jen -i bez hodnoty, vypíše se seznam aktivních rozhraní)
•	-p 23 (bude filtrování paketů na daném rozhraní podle portu; nebude-li tento parametr uveden, uvažují se všechny porty)
•	-t nebo --tcp (bude zobrazovat pouze TCP pakety)
•	-u nebo --udp (bude zobrazovat pouze UDP pakety)
•	--icmp (bude zobrazovat pouze ICMPv4 a ICMPv6 pakety)
•	--arp (bude zobrazovat pouze ARP rámce)
•	-n 10 (určuje počet paketů, které se mají zobrazit, tj. i "dobu" běhu programu; pokud není uvedeno, uvažujte zobrazení pouze jednoho paketu, tedy jakoby -n 1)
•	argumenty mohou být v libovolném pořadí

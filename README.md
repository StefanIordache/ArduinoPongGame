# ArduinoPongGame - Iordache Ștefan
Arduino Pong Game (single player)

Scurtă descriere:
    Jocul este unul single player, clasic, cu acumulare de puncte la lovirea bilei de paleta de joc, dificultatea constând în creșterea treptată a vitezei de mișcare a bilei. Se menționează faptul că viteza de mișcarea a paletei este constantă.
  
Descriere componente utilizate:
    Placa de dezvoltarea Arduino Uno, matrice 8x8 leduri, display lcd (16 coloane x 2 linii), joystick (din care s-au folosit mișcările pe o singură axă și apăsarea butonului pentru a începe jocul) + componente mici (jumper wires, condensatori, rezistențe).
    
Detalii privind modalitatea de joc:
    1. La conectarea plăcii la tensiune va fi afișat un mesaj pentru începerea jocului, alături de cel mai mare scor (0 la introducerea sub tensiune). Pentru a începe jocul efectiv este necesară apăsarea pe butonul manetei de control. Un cronometru de 3 secunde va oferi timp jucătorului pentru a ocupa poziția de joc. 
    2. La fiecare contact al paletei cu bila de joc se incrementează scorul curent. 
    3. Atunci când jucătorul nu reușește să lovească bila cu paleta jocul se oprește prin blocarea controlului, înghețarea matricii pentru a vedea poziția de final, iar după 2 sau 3 secunde se va merge la primul pas, după ce a fost consemnat scorul curent. În cazul în care acesta este mai mare decât scorul cel mai bun se va actualiza și se va reveni la pasul 1.
    
Detalii succinte despre implementare:
    1. Se folosesc 3 clase pentru a facilita implementarea: "dot", "pad", "game".
    2. Nu se folosește funcția "delay", ci funcția "millis" pentru a nu bloca activitatea plăcii.
    3. Clasa "dot" implementează controlul asupra bilei de joc prin poziția în raport cu axele de coordonate, viteza bilei și direcția (numerotată de la 0 la 7, unde 0 reprezintă nordul, iar 7 reprezintă nord-vestul, conform indicațiilor oferite de busolă)
    4. Clasa "pad" reglementează controlul asupra paletei prin reținerea și manipularea următoarelor date: poziția pe axa Ox (prin stânga și dreapta paletei) și dimensiunea ei.
    5. Clasa "game" urmărește starea jocului (GAME_ON, GAME_OFF, GAME_OVER), punctajele (cel curent, respectiv cel mai bun), dar și cronometrajul jocului (timpul de la apăsarea butonul de start, momentul la care s-a finalizat jocul și momentul curent al jocului).
    
Video demonstrativ:
    https://drive.google.com/open?id=1zFmojDi-ZzC8kQoBV7-dwYy65EdwF07V

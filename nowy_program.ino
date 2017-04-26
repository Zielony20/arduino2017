#define doTylu 0
#define doPrzodu 1
#define wLewo 2
#define wPrawo 3
#define ciutwLewo 4
#define ciutwPrawo 5
#define UcieczkaLewa 6
#define UcieczkaPrawa 7
#define obrotLewa 8

int czujnikLewyPrzod = 13;
int czujnikPrawyPrzod = 12;

//zmienne przechowujace aktualne wartosci czujnikow analogowych
int podlozeLewe,podlozePrawe;
int SharpLewy,SharpPrawy;
int czujnikZoltyLewy;
int czujnikZoltyPrawy;

//tablice do przechowania 3 ostatnich stanów czujników podloża
int bufforL[3]={1,1,1};
int bufforP[3]={1,1,1};
int Sample=0; //zmienna pomocnicza

int CzuloscPodloza = 0;
int SharpCzulosc = 85;


int E1 = 5;  //prawy motor
int M1 = 4;   //prawy PWM
int E2 = 6;   //lewy motor        
int M2 = 7;   //prawy PWM    
      
//przechowuje zadanie np wLewo
int Zadanie;
//moce silnika
int value1 = 40;
int value2 = 80;
int value3 = 128;
int value4 = 255;

//wyrownanie mocy silnikow
const int stalaSilnikowaL=1;
const int stalaSilnikowaP=1;


void poczatkowaAkcja(){
  }
void skanuj(){
  //zaczytanie wartosci czujnikow analogowych
  podlozePrawe = analogRead(A1);
  podlozeLewe = analogRead(A0);
  SharpLewy = analogRead(A4);
  SharpPrawy = analogRead(A3);
  czujnikZoltyLewy = !digitalRead(czujnikLewyPrzod);
  czujnikZoltyPrawy = !digitalRead(czujnikPrawyPrzod);


  //sprawdza 3 ostatnie próbki czujnikow podloza 
  bufforL[Sample]=analogRead(A0);
  bufforP[Sample]=analogRead(A1);
  Sample++;
  if(Sample >=3 ) Sample = 0;

//czujniki odleglosci
if(czujnikZoltyLewy==HIGH || czujnikZoltyPrawy==HIGH || SharpLewy > SharpCzulosc || SharpPrawy > SharpCzulosc)
{
    
    if( czujnikZoltyLewy==HIGH && czujnikZoltyPrawy==HIGH ){ Zadanie=doPrzodu;      Serial.println("<Dystans>Przeciwnik z przodu akcja ->do przodu"); }
    else if ( czujnikZoltyLewy==HIGH )                     { Zadanie=ciutwLewo;     Serial.println("<Dystans>Przeciwnik z przodu z lewej akcja ->ciutLewo");  }
    else if ( czujnikZoltyPrawy==HIGH )                    { Zadanie=ciutwPrawo;    Serial.println("<Dystans>Przeciwnik z przodu z prawej akcja ->ciutPrawo"); }
    else if ( SharpLewy > SharpCzulosc )                   { Zadanie=wLewo;         Serial.println("<Dystans>Przeciwnik z lewej akcja ->wLewo");     }
    else if ( SharpPrawy > SharpCzulosc )                  { Zadanie=wPrawo;        Serial.println("<Dystans>Przeciwnik z prawej akcja -> wprawo");    }
}
//czujniki podloza
else if (( podlozePrawe<=CzuloscPodloza && bufforP[0]<=CzuloscPodloza && bufforP[1]<=CzuloscPodloza && bufforP[2]<=CzuloscPodloza ) || (podlozeLewe<=CzuloscPodloza && bufforL[0]<=CzuloscPodloza && bufforL[1]<=CzuloscPodloza && bufforL[2]<=CzuloscPodloza  ))
  {
      if( podlozeLewe<=CzuloscPodloza && podlozePrawe<=CzuloscPodloza )     { Zadanie=doTylu;        Serial.println("<Podloze>Oba czujniki aktywne");  } //ucieczka gdy linia pod 2 czujnikami
      else if( podlozeLewe<=CzuloscPodloza )                                { Zadanie=UcieczkaLewa;  Serial.println("<Podloze>Czujnik lewy aktywny"); } //linia pod lewym czujnikiem
      else if( podlozePrawe<=CzuloscPodloza )                               { Zadanie=UcieczkaPrawa; Serial.println("<Podloze>Czujnik prawy aktywny"); } //linia pod prawym czujnikiem
  }

  
  else //ani przeciwnik ani linia
  {
    Serial.println("<------>nic");
    digitalWrite(M1,LOW);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1,255);   
    analogWrite(E2,255);    
  }
  
  
  }
void akcja(){
  switch(Zadanie){
  case doTylu: // czyli ucieczka z linii gdy aktywne sa 2 czujniki
  {
    Serial.println("<Akcja>wykonuje akcje ->doTylu");
    digitalWrite(M1,LOW);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1,255 * stalaSilnikowaL );   
    analogWrite(E2,40 * stalaSilnikowaP );
  }
  break;
  case doPrzodu:
  {
    Serial.println("<Akcja>wykonuje akcje ->doPrzodu");
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, 255 * stalaSilnikowaL);   
    analogWrite(E2, 255 * stalaSilnikowaP);
  }
  break;
  case wLewo:
  {
    Serial.println("<Akcja>wykonuje akcje ->wLewo");
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, LOW);       
    analogWrite(E1, 100 * stalaSilnikowaL);   
    analogWrite(E2, 0 * stalaSilnikowaP);
  }
  break;
  case wPrawo:
  {
    Serial.println("<Akcja>wykonuje akcje ->wPrawo");
    digitalWrite(M1,LOW);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, 0 * stalaSilnikowaL);   
    analogWrite(E2, 128 * stalaSilnikowaP);
  }
  break;
  case ciutwLewo: 
  {
    Serial.println("<Akcja>wykonuje akcje ->ciutwLewo");
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, 255 * stalaSilnikowaL);   
    analogWrite(E2, 0 * stalaSilnikowaP );
  }
  break;
  case ciutwPrawo:
  {
    Serial.println("<Akcja>wykonuje akcje ->ciutwPrawo");
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, 0 * stalaSilnikowaL );   
    analogWrite(E2, 255 * stalaSilnikowaP );
  }
    break;
  case UcieczkaPrawa:
  {
    Serial.println("<Akcja>wykonuje akcje ->UcieczkaPrawa");
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, 40 * stalaSilnikowaL );   
    analogWrite(E2, 255 * stalaSilnikowaP ); 
  }
  break;
  case UcieczkaLewa:
  {
    Serial.println("<Akcja>wykonuje akcje ->UcieczkaLewa");
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, HIGH);       
    analogWrite(E1, 255 * stalaSilnikowaL );   
    analogWrite(E2, 40 * stalaSilnikowaP);
  }
    break; 
  
  case obrotLewa: //obrot w lewą stronę

    Serial.println("<Akcja>wykonuje akcje ->obrotLewa");
    digitalWrite(M1,HIGH);   
    digitalWrite(M2, LOW);       
    analogWrite(E1, 255 * stalaSilnikowaL);   
    analogWrite(E2, 255 * stalaSilnikowaP);
  
  break;
  }
}


void setup() {
  Serial.begin(9600);
  
  pinMode(czujnikPrawyPrzod,INPUT); 
  pinMode(czujnikLewyPrzod,INPUT);
  
  pinMode(M1,OUTPUT);
  pinMode(M2,OUTPUT);
  
  delay(3600); // Odczekaj 5 sec
  poczatkowaAkcja(); // taktyczny start
}

void loop() {
  skanuj();
  akcja();
  delay(30);
}















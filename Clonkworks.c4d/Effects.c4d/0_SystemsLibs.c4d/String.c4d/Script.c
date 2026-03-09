#strict

/*
Funktionssammlung by TqmTqm
Rückgabetyp | Funktionskopf
--------------------------------------------------------------------------------------------
String      | ReplaceAll(string zeichenkette,string suchMuster,string ersetzenDurch)
String      | ReplaceFirst(string zeichenkette,string suchMuster,string ersetzenDurch)
String      | Substring(int start,string zeichenkette,int ende)
String      | CharAt(string zeichenkette,int i)	
String      | GetLetter(int zahl)
--------------------------------------------------------------------------------------------

ReplaceAll
	string zeichenkette -> die zu bearbeitende zeichenkette
	string suchMuster   -> die passage die zu ersetzten is
	string ersetzenDurch-> damit wird beim befund das suchmuster ersetzt
	
	bei ReplaceAll werden alle vorkommen von suchMuster im string zeichenkette durch
	ersetzenDurch ersetzt.
	
ReplaceFirst
	string zeichenkette -> die zu bearbeitende zeichenkette
	string suchMuster   -> die passage die zu ersetzten is
	string ersetzenDurch-> damit wird beim befund das suchmuster ersetzt
	
	bei ReplaceAll wird das erste suchMuster im string zeichenkette durch
	ersetzenDurch ersetzt.
	
Substringing
	int start			-> legt fest wo der neue string starten soll (0 ist ganz vorne)
	string zeichenkette -> die zu bearbeitende zeichenkette
	int ende(optional)  -> legt fest wo das ende des neue strings ist
	
	Substringing erzeugt aus einem string einen teilstring der durche einen bzw 2 
	parameter eingegrenzt wird. Hier dazu 2 Beispiele:
	
	Substring(1,"Clonk") -> "lonk"
	Substring(1,"Hamburger",4) -> "amb"
	
CharAt
	string zeichenkette -> die zu durchsuchende zeichenkette
	int i 				-> position (0 ist das erste zeichen)
	
	CharAt liefert den buchstaben an der position i zurück
	
GetLetter
	int zahl 			-> liefert das zeichen an der i der ascii tabelle
*/


global func ReplaceAll(string zeichenkette,string suchMuster,string ersetzenDurch){
	for (var i = 0;i<GetLength(zeichenkette);i++){
		if (suchMuster eq Substring(i,zeichenkette,GetLength(suchMuster)+i))
			zeichenkette = Format("%s%s%s",Substring(0,zeichenkette,i),ersetzenDurch,Substring(GetLength(suchMuster)+i,zeichenkette,GetLength(zeichenkette)));	
	}
	return(zeichenkette);
}
global func ReplaceFirst(string zeichenkette,string suchMuster,string ersetzenDurch){
	for (var i = 0;i<GetLength(zeichenkette);i++){
		if (suchMuster eq Substring(i,zeichenkette,GetLength(suchMuster)+i)){
			zeichenkette = Format("%s%s%s",Substring(0,zeichenkette,i),ersetzenDurch,Substring(GetLength(suchMuster)+i,zeichenkette,GetLength(zeichenkette)));	
			return(zeichenkette);
		}
	}
	return(zeichenkette);
}
global func Substring(int start,string zeichenkette,int ende){
	var neueZeichenkette = "";
	for (var i = 0;i<GetLength(zeichenkette) && (i < ende || ende == 0);i++){
		if (i >= start)
			neueZeichenkette = Format("%s%s",neueZeichenkette,GetLetter(GetChar(zeichenkette,i)));
	}
	return(neueZeichenkette);
}
global func CharAt(string zeichenkette,int i){
	return (GetLetter(GetChar(zeichenkette,i)));
}
global func GetLetter(int zahl){
	var asciiArray = [" ","?","!","''","#","$","%","&","'","(",")","*","+",",","-",".","/","0","1","2","3","4","5","6","7","8","9",":",";","<","=",">","?","@","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","[","Backslash","]","^","`","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","{","|","}","~"];
	return(asciiArray[zahl-32]);
}
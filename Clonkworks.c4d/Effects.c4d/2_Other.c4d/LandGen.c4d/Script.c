/*-- LandGen --*/
/* By Gecko .. */

/* Erstellt eigene Landschaften. Dynamisch. aber doch ein wenig Statisch. Der Cloue:
und LandGen macht dir Landschaft. Es Gibt LandGenPunkte, die nur Gott sieht, also Du.
Punkte kannst du sagen ob sie Fest sein sollen, oder Weich. Oder ob sie halt zufall sein sollen.
Naja. und du kannst ihnen auch direkt sagen, was sie sein sollen. Dann die Rauheit und soweiter. */

#strict


/* - Initialisierung - */
func Initialize() {

  return(1);
}
global func CreateAScape(object obj,string mat,int iR,int iSize,int iRandom1,int iRandom2, int sub,int iRepeats) {
for(var i = 0; i <= iRepeats; i++){
  var R = iR;
  var x = iSize;
  var y = iSize;
  var u1 = -Sin(R-RandomX(iRandom1,iRandom2),x/2)+Cos(R-RandomX(iRandom1,iRandom2),-y/2);
  var v1 =  Cos(R-RandomX(iRandom1,iRandom2),x/2)-Sin(R-RandomX(iRandom1,iRandom2),y/2);
 
  var u2 = +Sin(R-RandomX(iRandom1,iRandom2),x/2)+Cos(R-RandomX(iRandom1,iRandom2),-y/2);
  var v2 = -Cos(R-RandomX(iRandom1,iRandom2),x/2)-Sin(R-RandomX(iRandom1,iRandom2),y/2);  

  var u3 = +Sin(R-RandomX(iRandom1,iRandom2),x/2)-Cos(R-RandomX(iRandom1,iRandom2),-y/2);
  var v3 = -Cos(R-RandomX(iRandom1,iRandom2),x/2)+Sin(R-RandomX(iRandom1,iRandom2),y/2);  

  var u4 = -Sin(R-RandomX(iRandom1,iRandom2),x/2)-Cos(R-RandomX(iRandom1,iRandom2),-y/2);
  var v4 =  Cos(R-RandomX(iRandom1,iRandom2),x/2)+Sin(R-RandomX(iRandom1,iRandom2),y/2);  
 
DrawMaterialQuad(mat,GetX(obj)-u1,GetY(obj)-v1,GetX(obj)-u2,GetY(obj)-v2,GetX(obj)-u3,GetY(obj)-v3,GetX(obj)-u4,GetY(obj)-v4,sub);
 }
}

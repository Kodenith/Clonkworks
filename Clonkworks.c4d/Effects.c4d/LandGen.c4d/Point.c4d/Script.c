/*-- LandGen Point --*/

#strict

public func WahrscheinlichkeitImRandomFuerScheissMat()  {   return(20); }

local iMod;
local iSize;
local iR;
local iRandom1;
local iRandom2;
local isub;
local iRepeats;
local stMat;
local NoRandiSetMehr;
local TuEffect;

func Initialize() {
   iMod = 1;
   if(!Random(3)){ iMod = RandomX(2,3); }
   iSize = RandomX(10,40);
   iR = 0;
   iRandom1 = -50;
   iRandom2 = 50;
   isub = 1;
   iRepeats=5;
   UpdateGraph();
  return(1);
}
public func UpdateGraph() {
 if(iMod == 1) SetGraphics("Randi");
 if(iMod == 2) SetGraphics("Fest");
 if(iMod == 3) SetGraphics("Weich");
 if(iMod == 4) SetGraphics("Liq");
 if(iMod == 5) SetGraphics("Fest");   
  return(1);
}
/* Okay, hier wirds nun ein wenig kompliziert:
iMod (1 = Random, 2=Fest,3=Weich,4=Liquid,5=Definiert)
iSize (Größe des Feldes)
iR (Drehung der entstehende Fläche)
iRandom1 untergrenze
iRandom2 obergrenze (ändern den iR)
iRepeats = repeats. sollten schon ordentlich groß sein. so 5 bis 20
isub unterirdisch?
falls iMod = 5 -> stMat
*/

public func Set(int Mod,int Size,int R, int Random1, int Random2,int Repeats, sub, string Mat, int effect) {
   iMod = Mod;
   iSize = Size;
   iR = R;
   iRandom1 = Random1;
   iRandom2 = Random2;   
   iRepeats = Repeats;
   isub = sub;
   stMat = Mat;
   UpdateGraph();
   TuEffect = effect;
  return(1);
}
// object obj,string mat,int iR,int iSize,int iRandom1,int iRandom2, int sub,int iRepeats
public func CreateMyScape(){
  // Naja am Start sollte die Insel nicht eingebaut werden, wenn die ganze Map später Spawnt
 // if(StartRdy){ if(Distance(GetX(),GetY(),StartX,StartY) < 300){ RemoveObject(); return(0); } }
  
  if(!stMat){ CreateRandomMat(); }
  CreateAScape(this(),stMat,iR,iSize,iRandom1,iRandom2,isub,iRepeats);
  if(TuEffect) CreateParticle("GhostSpark",0,0,RandomX(-2,2),-4,iSize*10+RandomX(0,iSize/2),RGB(255,255,255));
  RemoveObject();
  return(1);
}
// Appendto stelle für neue Mats
public func CreateRandomMat(){
if(NoRandiSetMehr) return(0);
if(iMod == 1){
  if(!Random(WahrscheinlichkeitImRandomFuerScheissMat())){
   var iMax = 20;
   var iRnd = RandomX(0,13);
   if(iRnd == 0) { stMat = "Ashes";   }
   if(iRnd == 1) { stMat = "Acid";   }
   if(iRnd == 2) { stMat = "Coal";   }
   if(iRnd == 3) { stMat = "Crystal";   }
   if(iRnd == 4) { stMat = "Earth";   } 
   if(iRnd == 5) { stMat = "FlyAshes";   }
   if(iRnd == 6) { stMat = "FlySand";   }
   if(iRnd == 7) { stMat = "Gold";   }
   if(iRnd == 8) { stMat = "Granite";   }
   if(iRnd == 9) { stMat = "Ore";   }    
   if(iRnd ==10) { stMat = "Rock";   }
   if(iRnd ==11) { stMat = "Sand";   }
   if(iRnd ==12) { stMat = "Sulphur";   }
   if(iRnd ==13) { stMat = "Water";   }            
   iRandom1 = -50;
   iRandom2 = 50;
   }
   var iMax = 20;
   var iRnd = 1;
   if(iRnd == 1) {
    stMat = "Earth"; 
   iRandom1 = -20;
   iRandom2 = 20;
      }   
  }
if(iMod == 2){
   var iRnd = RandomX(0,4);
   if(iRnd == 0) { stMat = "Gold";   }
   if(iRnd == 1) { stMat = "Ore";   }
   if(iRnd == 2) { stMat = "Rock";   }
   if(iRnd == 3) { stMat = "Granite";   }
   if(iRnd == 4) { stMat = "Crystal";   }          
   iRandom1 = -20;
   iRandom2 = 100;
   if(Random(4)){
   var pOther = FindObject2(Find_ID(GetID(this()),Find_Distance(iSize*2)));
    if(pOther){ pOther->SetMat(stMat); }
    } 
   } 
if(iMod == 3){
   var iRnd = RandomX(0,3);
   if(iRnd == 0) { stMat = "Sulphur";   }
   if(iRnd == 1) { stMat = "Coal";   }
   if(iRnd == 2) { stMat = "Sand";   }   
   if(iRnd == 3) { stMat = "Earth";   }   
   iRandom1 = -50;
   iRandom2 = 50;
   if(Random(4)){
   var pOther = FindObject2(Find_ID(GetID(this()),Find_Distance(iSize*2)));
    if(pOther){ pOther->SetMat(stMat); }
    }  
     } 
}
public func SetMat(string doof){
stMat = doof;
NoRandiSetMehr = 1;
}

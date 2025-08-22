/*-- Neues Szenario --*/

#strict

func Initialize() {
  FindObject2(Find_ID(FALW))->AddForgeable(METL, RGBa(177, 171, 170, 0), 3, 34, 2, "Meltable");
  FindObject2(Find_ID(FALW))->AddForgeable(CPIG, RGBa(170, 79, 66, 0), 4, 30, 3, "Meltable");
  FindObject2(Find_ID(FALW))->AddForgeable(TTIG, RGBa(120, 130, 139, 0), 2, 37, 0, "");
  
  ScriptGo(1);
  
  return(1);
  
}

func Script1(){
  var house = FindObject2(Find_ID(HUT3));
  AddEffect("Restock",house, 50, 1,house);
}

global func FxRestockTimer(object pTarget, int EffectNumber){
	var Itemlist;
	Itemlist = [WOOD, METL, CPIG, TTIG, LORY, SPHR, CRYS, ARWP, FLNT, GUNP, CNKT, LNKT, CANN, XBOW, ADM3];
	
	for(var id in Itemlist){
		if(ContentsCount(id, this()) == 0){
			CreateContents(id, this(), 1);
		}
	}
}

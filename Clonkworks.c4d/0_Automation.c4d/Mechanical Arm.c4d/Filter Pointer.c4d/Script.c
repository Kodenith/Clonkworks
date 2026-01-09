/*-- Neues Script --*/

#strict 2
local Filt;
local Station;

func SetGraph(){
	var FilterImage = Filt;
  //main
  SetGraphics(0,this(),FilterImage,1,4);
  SetObjDrawTransform(700,0,((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/8,0,700,GetDefHeight(GetID())*1000/8+(((GetDefCoreVal("Picture","DefCore",FilterImage,2)/2)*1000)/8)-10000,this(),1);
}

func Set(){
	Filt = Par(0);
	SetGraph();
}

func GetUseable(){
	var FoundItems = FindObjects(Find_Distance(150,AbsX(GetX(Station)),AbsY(GetY(Station))),Find_ID(Filt),Find_Not( Find_Distance(40)),Sort_Distance());
	if(Filt == MAM5){
		FoundItems = FindObjects(Find_Distance(150,AbsX(GetX(Station)),AbsY(GetY(Station))),Find_OCF(OCF_Collectible),Find_Not( Find_Distance(40)),Sort_Distance(),Find_Not(Find_ID(FLAG)),Find_Not(Find_OCF(OCF_Alive)));
		for(var i in LocalN("PointerList",Station)){
			if(i != this()) for(var j in FoundItems){
				if(GetID(j) == LocalN("Filt",i)) ArrayDeleteEntry(FoundItems,j);
			}
		}
	}
	
	var Found;
	for(var i in FoundItems){
		if(Contained(i)){
			if(!LocalN("GrabFromContainer",Station)) continue;
			if(GetOCF(Contained(i)) & OCF_Alive) continue;
			if(Abs(GetXDir(Contained(i))) >= 5 || Abs(GetYDir(Contained(i))) >= 5) continue;
		}
		if(GetPathLength(GetX(Station),GetY(Station),GetX(i),GetY(i)) == 0) continue;
		if(GetCon(i) < 100 && !GetDefIncompleteActivity(GetID(i))) continue;
		if(!Stuck(i) && Abs(GetXDir(i)) < 5 && Abs(GetYDir(i)) < 5 && !ObjectOnClaw(i) && !ObjectOnConveyor(i)){
			Found = i;
			break;
		}
	}
	if(Found) return(Found);
}
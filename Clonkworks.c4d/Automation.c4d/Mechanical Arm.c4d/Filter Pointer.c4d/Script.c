/*-- Neues Script --*/

#strict 2
local Filt;
local Avoid;

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
	var FoundItems = FindObjects(Find_NoContainer(),Find_Distance(180,AbsX(GetX(Avoid)),AbsY(GetY(Avoid))),Find_ID(Filt),Find_Not( Find_Distance(50)),Sort_Distance());
	var Found;
	for(var i in FoundItems){
		if(!Stuck(i)){
			Found = i;
			break;
		}
	}
	if(Found) return(Found);
}
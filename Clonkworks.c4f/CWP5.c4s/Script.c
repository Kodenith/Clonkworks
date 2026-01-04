/*-- Neues Szenario --*/

#strict

func Initialize() {
	DrawMaterialQuad("Gold-Rough", 0,0, LandscapeWidth()-1,0, LandscapeWidth()-1,LandscapeHeight()-1, 0,LandscapeHeight()-1);
	Schedule("begin()",1,0);
  return(1);
}

global func begin(){
	CreateObject(___L)->SetOutset();
}

func InitializePlayer(int iPlr){
	PlrMessage("$TxtWelcome$",iPlr);
	SetPosition(GetX(FindObject(HUT3)), GetY(FindObject(HUT3)), GetCrew(iPlr,0));
	var flag = CreateObject(FLAG, 130+(5*iPlr),509, 0);
	Enter(FindObject(HUT3),flag);
	SetOwner(iPlr, flag);
	SetFoW (0, iPlr);
	SetPlrKnowledge(iPlr, PGTP);
	SetPlrKnowledge(iPlr, KDNT);
	var i, id; while (id = GetDefinition(i++)){
		if((GetCategory(,id) & C4D_Structure) && !(GetCategory(,id) & C4D_Knowledge)) continue;
		if((GetCategory(,id) & C4D_Vehicle) && !(GetCategory(,id) & C4D_Knowledge)) continue;
		SetPlrKnowledge(iPlr, id);
		var ok = false;
		var def = id;
		if(GetCategory(,def) & 512) ok = true;
		if(GetCategory(,def) & 1024) ok = true;
		if(GetCategory(,def) & 2048) ok = true;
		if(GetCategory(,def) & 16384) ok = true;
		if(GetDefCoreVal("CrewMember", "DefCore", def)) ok = true;
		if(GetCategory(,def) & C4D_Structure) ok = false;
		if(GetComponent(DUMM,,,def) > 0) ok = false;
		if(!ok) continue;
		DoHomebaseMaterial(iPlr,id,999);
	}
}

public func RelaunchPlayer(Owner){
	if(!FindObject(___S)) return(0);
	var conk = CreateObject(CLNK,LandscapeWidth()/2,LandscapeHeight()/3);
	var Cabin = FindObject(HUT3);
	if(Cabin) Enter(Cabin,conk);
	
	SetOwner(Owner,conk);
	MakeCrewMember(conk,Owner);
	SetCursor(Owner,conk);
}

//if the players summon meteors, might aswell be random :)
global func GiantMeteoriteType(){ 
var list=[RDEP,CDEP,ODEP,TDEP,GDEP];
return(list[Random(4)]);
}
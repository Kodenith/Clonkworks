
#strict 2

func Initialize(){
	SetPosition(0,0);
}

func DoVote(){
	//return(1);
	return(IsNetwork());
}

func Activate(iPlr){
	var pClonk = GetCursor(iPlr);
	CreateMenu(GetID(),pClonk,this(),0,GetName(),,1);
	AddMenuItem("$TxtInfo$",,,pClonk);
	AddMenuItem("$O1$","DoOutset",LOAM,pClonk,0,iPlr);
	AddMenuItem("$O2$","DoMountains",ROCK,pClonk,0,iPlr);
	AddMenuItem("$O3$","DoCaves",CRYS,pClonk,0,iPlr);
	AddMenuItem("$O4$","DoChasm",LAVA,pClonk,0,iPlr);
	AddMenuItem("$O5$","DoIslands",FXP1,pClonk,0,iPlr);
	//AddMenuItem("$O6$","DoWater",WBRL,pClonk,0,iPlr);
}

func DoMapChange(){
	
	//resetting stuff
	for(var i in FindObjects(Find_Or( Find_Category(C4D_Object), Find_Category(C4D_Structure), Find_Category(C4D_Vehicle),Find_Category(C4D_Living),Find_Category(C4D_StaticBack)), Find_Not( Find_Category(C4D_Goal), Find_Category(C4D_Rule),Find_Category(64)))){
		if(GetCategory(i) & C4D_Goal) continue;
		if(GetCategory(i) & C4D_Rule) continue;
		if(GetCategory(i) & 64) continue;
		RemoveObject(i);
	}
	CreateObject(EGRS);
	
	//recreate clonks
	for(var i = 0; i<GetPlayerCount(); i++){
		var iPlr = GetPlayerByIndex(i);
		var Placement = PlaceAnimal(WIPF);
		if(!Placement) continue;
		var Clonk = CreateObject(CLNK,GetX(Placement),GetY(Placement),iPlr);
		RemoveObject(Placement);
		MakeCrewMember(Clonk,iPlr);
		SetCursor(iPlr,Clonk);
		PlaceAnimal(HUT3);
		var hut = FindObject(HUT3);
		if(hut){
			Enter(hut,GetCursor(iPlr));
			var Flag = CreateObject(FLAG,0,0,iPlr);
			Enter(hut,Flag);
		}
	}
	
	//Recreate Landscape Deco
	PlaceObjects(LOAM,15+(5*GetPlayerCount()),"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	PlaceObjects(ROCK,15+(5*GetPlayerCount()),"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	PlaceObjects(FLNT,15+(5*GetPlayerCount()),"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	PlaceObjects(GOLD,15+(5*GetPlayerCount()),"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	PlaceObjects(RDEP,5+(5*GetPlayerCount()),"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	PlaceObjects(MEGG,GetPlayerCount(),"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	for(var i = 0; i < 15+(GetPlayerCount()*5); i++){
		var Placement = PlaceAnimal(WIPF);
		CreateObject(TRE1,GetX(Placement),GetY(Placement)+18);
		RemoveObject(Placement);
	}
	for(var i = 0; i < 6+(GetPlayerCount()*3); i++){
		var Placement = PlaceAnimal(WIPF);
		CreateObject(TRE2,GetX(Placement),GetY(Placement)+18);
		RemoveObject(Placement);
	}
	for(var i = 0; i < GetPlayerCount(); i++){
		var Placement = PlaceAnimal(WIPF);
		CreateObject(BUSH,GetX(Placement),GetY(Placement)+18);
		RemoveObject(Placement);
	}
	
	//Wipfs for good measure
	PlaceObjects(WIPF,5,"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight(),1);
	//CreateObject(___L);
	
	//Remove duplicate clonks incase
	for(var i = 0; i<GetPlayerCount(); i++){
		var iPlr = GetPlayerByIndex(i);
		for(var j in FindObjects(Find_OCF(OCF_CrewMember), Find_Owner(iPlr))){
			if(GetCursor(iPlr) != j) RemoveObject(j);
		}
	}
}

//Outset
func DoOutset(){
	if(DoVote()){
		CastVote(Par(1)+1,"Change landscape to Outset?","SetOutset");
	}else{
		SetOutset();
	}
}

public func SetOutset(){
	Log("Map: $O1$");
	DrawDefMap(0,0,LandscapeWidth(),LandscapeHeight(),"Outset");
	DoMapChange();
	return(1);
}
//Mountains
func DoMountains(){
	if(DoVote()){
		CastVote(Par(1)+1,"Change landscape to Mountains?","SetMountains");
	}else{
		SetMountains();
	}
}

public func SetMountains(){
	Log("Map: $O2$");
	DrawDefMap(0,0,LandscapeWidth(),LandscapeHeight(),"Mountains");
	DoMapChange();
	PlaceObjects(ODEP,2,"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	PlaceObjects(CDEP,2,"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	return(1);
}
//Caves
func DoCaves(){
	if(DoVote()){
		CastVote(Par(1)+1,"Change landscape to Caves?","SetCaves");
	}else{
		SetCaves();
	}
}

public func SetCaves(){
	Log("Map: $O3$");
	DrawDefMap(0,0,LandscapeWidth(),LandscapeHeight(),"Caves");
	DoMapChange();
	PlaceObjects(GDEP,5,"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	return(1);
}
//Chasm
func DoChasm(){
	if(DoVote()){
		CastVote(Par(1)+1,"Change landscape to Chasm?","SetChasm");
	}else{
		SetChasm();
	}
}

public func SetChasm(){
	Log("Map: $O4$");
	DrawDefMap(0,0,LandscapeWidth(),LandscapeHeight(),"Vulkan");
	DoMapChange();
	PlaceObjects(GDEP,5,"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	return(1);
}

//Islands
func DoIslands(){
	if(DoVote()){
		CastVote(Par(1)+1,"Change landscape to Islands?","SetIslands");
	}else{
		SetIslands();
	}
}

public func SetIslands(){
	Log("Map: $O5$");
	DrawDefMap(0,0,LandscapeWidth(),LandscapeHeight(),"Skylands");
	DoMapChange();
	PlaceObjects(GDEP,5,"GBackSolid",0,0,LandscapeWidth(),LandscapeHeight());
	return(1);
}

//Water
func DoWater(){
	if(DoVote()){
		CastVote(Par(1)+1,"Change landscape to Tropical?","SetWater");
	}else{
		SetWater();
	}
}

public func SetWater(){
	Log("Map: $O5$");
	DrawDefMap(0,0,LandscapeWidth(),LandscapeHeight(),"Islands");
	DoMapChange();
	return(1);
}

public func flood(){
	CastPXSX("Water", 60000, 30,LandscapeWidth()/2,100,90);
}
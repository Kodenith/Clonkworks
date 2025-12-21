
#strict 2

func Initialize(){
	SetPosition(0,0);
}

func DoVote(){
	//return(1);
	return(IsNetwork());
}

func Activate(iPlr){
	var goalList=[GLDM,OREM,ENGI,MELE,MNTK,OILP,WPHT,CI5P,COPM,TITM];
	var pClonk = GetCursor(iPlr);
	CreateMenu(GetID(),pClonk,this(),0,GetName());
	for(var i in goalList){
	AddMenuItem(GetName(,i),"DoGoal",i,pClonk,0,iPlr);
	}
}

func DoGoal(Goal,Plr){
	if(DoVote()){
		CastVote(Plr+1,Format("Begin Goal: %s",GetName(,Goal)),"StartGoal(%v)",Goal);
	}else{
		StartGoal(Goal);
	}
}

global func StartGoal(gl){
	var g = CreateObject(gl);
	for(var i in FindObjects(Find_Category(C4D_Goal),Find_Exclude(g))){
		RemoveObject(i);
	}
	for(var i = 0; i < GetPlayerCount(); i++){
		SetWealth(GetPlayerByIndex(i),125);
	}
	
	var RemoveThese = [TACC,THBA,TKNW,REAC,NMGE];
	for(var i in RemoveThese) RemoveObject(FindObject(i));
	CreateObject(CNMT); CreateObject(ENRG);
	Log("Goal Started!");
}
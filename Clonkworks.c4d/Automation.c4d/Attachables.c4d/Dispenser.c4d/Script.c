/*-- Neues Script --*/

#strict 2
#include CHTE

private func GetAttachable(){
	var Objects = FindObjects(Find_AtPoint(),Find_NoContainer(),Find_Or( Find_Category(C4D_Structure), Find_Category(C4D_Vehicle)));
	for(var object in Objects){
		var banned = [CMIX,QUAR,REFU,CT5P,ZIPP,MTBK,PONT,PNT2,ANVL,DRCK,FNDR,HUT1,IGLO,TWR1,TWR2,CATA,XBOW,WAGN,FTG1,FTG2,FTG3,PPIN,PPOU];
		if(InArray(GetID(object),banned) != -1) continue;
		if(GetCon(object) < 100) continue;
		if(OnFire(object)) continue;
		
		var stop = false;
		for(var i in FindObjects(Find_Func("ObjectAttachable"))){
			if(LocalN("Target",i) == object) stop = true;
		}

		if(stop) continue;
		
		//checks for vehicles only
		if(GetCategory(object) & C4D_Vehicle){
			continue;
		}
		
		if(GetDefCoreVal("Collection","DefCore",GetID(object))) continue;
		
		///checks for structures only
		if(GetCategory(object) & C4D_Structure){
			if(GetR(object) != 0) continue;
			if(GetDefCoreVal("Entrance","DefCore",GetID(object)) == 0) continue;
		}
		
		return(object);
	}
}

func Activate(pClonk){
	[$TxtAttach$]
	var Obj = GetAttachable();
	if(!Obj){
		Message("$TxtNoFound$",Contained());
		Sound("CommandFailure1");
		return(1);
	}
	DoAttach(DSP2,Obj);
	return(1);
}
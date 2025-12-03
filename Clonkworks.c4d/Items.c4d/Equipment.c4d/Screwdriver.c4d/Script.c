/*-- Neues Script --*/

#strict 2

func Hit(){
	if(!Random(3)) Sound("MetalHit*");
}

public func Activate(pClonk){
	[$TxtUnscrew$]
	
	//unscrewing
	var Unscrewable = FindObject2(Find_OnLine(0,0,0,10), Find_Func("Unscrewable"), Find_NoContainer(), Find_Exclude(this()));
	if(Unscrewable != 0){
		Unscrew(Unscrewable);
		return(1);
	}
	
	//line tweaking
	var LineObj = FindObject2(Find_NoContainer(),Find_OCF(OCF_LineConstruct),Find_AtPoint());
	if(!LineObj) return(0);
	var Lines = [];
	//finding all lines conencted to LineObj
	for(var i in FindObjects(Find_Func("isLine"))){
		if(GetActionTarget(0,i) == LineObj || GetActionTarget(1,i) == LineObj){
			ArrayAdd(Lines,i);
		}
	}
	
	//DebugLog("%v",Lines);
	
	//Line menu, all lines that return 1 to LineTweakable get added to it.
	CreateMenu(SCWD,pClonk,this(),0,"$TxtNoTweakable$");
	PlayerMessage(GetOwner(pClonk),Format("$TxtTweakInfo$",GetName(LineObj)),pClonk);
	for(var line in Lines){
		if(line->~LineTweakable()){
			AddMenuItem("$TxtTweak$","StartLineTweak",GetID(line),pClonk,0,line);
		}
	}
	
	return(1);
}

public func StartLineTweak(foo,pLine){
	if(!pLine) return(0);
		
	var pClonk = Contained();
	pLine->~TweakMenu(pClonk);
	Sound("screw*");
}

public func Unscrew(pObj){
	if(!pObj) return(0);

	pObj->~Unscrew();
	Sound("screw*");
	return(1);
}

public func IsAnvilProduct(){ return(1); }
public func IsAdvancedProduct(){ return(1); }

public func Initialize(){ SetCon(150); }
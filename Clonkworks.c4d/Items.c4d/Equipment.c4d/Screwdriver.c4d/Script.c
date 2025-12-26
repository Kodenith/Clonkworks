/*-- Neues Script --*/

#strict 2

func Hit(){
	if(!Random(3)) Sound("MetalHit*");
}

public func Activate(pClonk){
	[$TxtUnscrew$]
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

public func IsAnvilProduct(){ return(1); }
public func IsAdvancedProduct(){ return(1); }

public func Initialize(){ SetCon(150); }
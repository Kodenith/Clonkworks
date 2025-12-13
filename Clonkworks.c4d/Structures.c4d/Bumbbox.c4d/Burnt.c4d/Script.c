/*-- Neues Script --*/

#strict 3
#include HNCB

local DesignatedBumb2;

func Initialize() {
  return(1);
}

func Update(){
	if(DesignatedBumb != nil && GetID(DesignatedBumb) == BUMB)
		DesignatedBumb.Comb = 0;
	if(DesignatedBumb2 != nil &&  GetID(DesignatedBumb2) == BUMB)
		DesignatedBumb2.Comb = 0;
}

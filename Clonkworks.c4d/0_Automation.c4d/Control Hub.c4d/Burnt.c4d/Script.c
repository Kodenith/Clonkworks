/*-- Neues Script --*/

#strict 2
#include BAS9

func Incineration(){
    for(var i in FindObjects(Find_Distance(650),Find_Func("AutoOrderComp"),Find_NoContainer(),Find_Not(Find_Hostile(GetOwner())))){
        RemoveEffect("OrderMode",i);
    }
}
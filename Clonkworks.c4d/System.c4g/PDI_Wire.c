#strict 2
#appendto WRNC
#appendto WRKT
#appendto CC1_
#appendto CC3_
#appendto CC4_
#appendto CC5_
#appendto HATC
#appendto I1_L
#appendto MGAT
#appendto I5_M
#appendto PIS1
#appendto I1_P
#appendto SPIK
#appendto SRIK
#appendto CLBD
#appendto ECAN
#appendto EBOW
#appendto ELRY
#appendto HUMD
#appendto RCBP

public func PDI_Wire(){ return(1); }

public func PDI_Children(){
    if(GetID(this)->~UsedCircuits()) return(GetID(this)->~UsedCircuits());
}
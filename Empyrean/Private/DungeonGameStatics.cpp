// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGameStatics.h"


bool UDungeonGameStatics::GetFloatParameterByName(UObject* Target, FName Name, float& OutFloat)
{
    if (Target)
    {
        float FoundFloat;
        FFloatProperty* FloatProp = FindFProperty<FFloatProperty>(Target->GetClass(), Name);
        if (FloatProp)
        {
            FoundFloat = FloatProp->GetPropertyValue_InContainer(Target);
            OutFloat = FoundFloat;
            return true;
        }
    }
    return false;
}

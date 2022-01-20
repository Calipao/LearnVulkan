#pragma once

//Indices of Queue families

struct QueueFamilyIndices
{
    int graphicsFamily = -1;    //Location of graphics queue family

    bool IsValid()
    {
        return graphicsFamily >= 0;
    }
};
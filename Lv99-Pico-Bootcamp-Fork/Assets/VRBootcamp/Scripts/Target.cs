using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Target : MonoBehaviour
{
    public int HorizontalDirection;

    private void Awake()
    {
        // Randomise Movement Directions
        if ((int)Random.Range(0, 2) == 0)
            HorizontalDirection = -1;
        else
            HorizontalDirection = 1;
    }
}

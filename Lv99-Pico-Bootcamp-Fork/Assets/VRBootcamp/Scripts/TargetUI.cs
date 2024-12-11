using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class TargetUI : MonoBehaviour
{
    [SerializeField] Text ScoreText;

    public void UpdateScore(int score)
    {
        ScoreText.text = "Score: " + score.ToString();
    }
}

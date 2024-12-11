using System.Text;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class LogController : MonoBehaviour
{

    [SerializeField]
    private TextMeshProUGUI logText;
    private StringBuilder logStringBuffer;
    private ScrollRect scrollRect;

    private static LogController instance;

    public Transform attachPoint;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
        }
        else 
        {
            LogError("A LogController instance already exists.");
        }
    }

    private void Start()
    {
        if (logText != null)
        {
            logText.text = string.Empty;
            logStringBuffer = new StringBuilder(1000);
            scrollRect = instance.logText.GetComponent<ScrollRect>();
        }
    }

    public static void Log(string text)
    {
        Debug.Log($"PICO VR Bootcamp:{text}");

        if (instance == null || instance.logText == null || instance.logStringBuffer == null) return;

        instance.logText.text = instance.logStringBuffer.AppendLine(text).ToString();
        instance.scrollRect.normalizedPosition = Vector2.zero;
    }

    public static void LogError(string text)
    {
        Debug.LogError($"PICO VR Bootcamp:{text}");

        if (instance == null || instance.logText == null || instance.logStringBuffer == null) return;

        instance.logText.text = instance.logStringBuffer.AppendLine($"<color=\"red\">{text}</color>").ToString();
        instance.scrollRect.normalizedPosition = Vector2.zero;
    }

    public void Hide()
    {
        this.gameObject.SetActive(false);
    }

}

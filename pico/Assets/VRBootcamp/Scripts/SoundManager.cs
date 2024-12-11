using UnityEngine;


[RequireComponent(typeof(AudioSource))]
public class SoundManager : MonoBehaviour
{
    private static SoundManager _instance;
    public static SoundManager instance
    {
        get => _instance;
        private set
        {
            if (_instance != null)
            {
                Debug.LogWarning("Second attempt to get SoundManager.");
            }
            _instance = value;
        }
    }

    [Header("Audiclips for interactions")]
    public AudioClip audiConfirm;
    public AudioClip audiHoverTick;
    public AudioClip audiError;

    private AudioSource audiSource;

    private void Awake()
    {
        instance = this;
        audiSource = GetComponent<AudioSource>();
        audiSource.loop = false;//You don't want this audiosource to loop any sound clips
    }

    //You can leave start method blank here so you can still turn enable/disable in the inspector
    void Start() 
    {}

    public void PlayConfirm()
    {
        if(audiConfirm != null)
            audiSource.PlayOneShot(audiConfirm);
    }

    public void PlayHoverTick()
    {
        if (audiHoverTick != null)
            audiSource.PlayOneShot(audiHoverTick);
    }

    public void PlayError()
    {
        if (audiError != null)
            audiSource.PlayOneShot(audiError);
    }

}

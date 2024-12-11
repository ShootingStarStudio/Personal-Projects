using Unity.VisualScripting;
using UnityEngine;

public class Furnace : MonoBehaviour
{
    CoalTrigger coalTrigger;
    OreTrigger oreTrigger;

    GameObject spawnpoint;

    // Start is called before the first frame update
    void Start()
    {
        coalTrigger = transform.GetChild(0).GetComponent<CoalTrigger>();
        oreTrigger = transform.GetChild(1).GetComponent<OreTrigger>();
        spawnpoint = transform.GetChild(2).gameObject;
    }

    // Update is called once per frame
    void Update()
    {
        if (coalTrigger.coalstorage.Count>0 && oreTrigger.orestorage.Count>0) {
            coalTrigger.smelt();
            oreTrigger.smelt();

            GameObject ingot = Instantiate(Resources.Load<GameObject>("Project/Prefabs/Ingot"), spawnpoint.transform);
            ingot.transform.parent = null;
            ingot.transform.localScale = Vector3.one;
        }
    }
}

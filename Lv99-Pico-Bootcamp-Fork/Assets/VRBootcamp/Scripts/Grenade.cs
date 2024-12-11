using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Grenade : MonoBehaviour
{
    [SerializeField] GameObject GrenadeBody;
    [SerializeField] ParticleSystem PSys;
    [SerializeField] GrabbedObject grabbedObject;
    bool bCountdown = true; // Whether Counting Down
    float fCountdown = 5.0f; // Time Till Grenade Explodes
    bool ExplodeStart = false;

    List<Target> TargetsInRange = new List<Target>();

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Target")
        {
            TargetsInRange.Add(other.gameObject.GetComponent<Target>());
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.tag == "Target")
        {
            TargetsInRange.Remove(other.gameObject.GetComponent<Target>());
        }
    }

    private void Update()
    {
        bCountdown = grabbedObject.Released;
        ExplodeUpdate();
    }

    // Update for Explosion Checks
    void ExplodeUpdate()
    {
        if (bCountdown) // If Active
        {
            fCountdown -= Time.deltaTime; // Counting down
            if (fCountdown <= 0) // If countdown is over
            {
                Explode(); // The Act of Exlploding
            }
        }
    }

    // Explosion of grenade
    void Explode()
    {
        if (!ExplodeStart)
        {
            PSys.Play(); // Start Playing Explosion Particles
            GrenadeBody.SetActive(false); // Body Active False
            ExplodeStart = true;
            for (int i = 0; i < TargetsInRange.Count; i++)
            {
                Target target = TargetsInRange[i];
                if (target.isActiveAndEnabled)
                {
                    target.gameObject.SetActive(false); // Make it Disappear 
                    target.transform.parent.GetComponent<ShootingTargetField>().AddScore(); // Add Score
                }
            }
        }
        else if (PSys.isStopped)
        {
            Destroy(gameObject);
        }
    }

}

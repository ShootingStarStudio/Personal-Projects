using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.Interaction.Toolkit;

public class Shoot : MonoBehaviour
{

    public GameObject bullet;
    public ParticleSystem muzzleFlash;
    public Transform spawnPoint;
    public float fireSpeed = 20;
    bool shootingRifle = false; // Bool to check if shooting rifle is true or false


    // Start is called before the first frame update
    void Start()
    {
        XRGrabInteractable grabbable = GetComponent<XRGrabInteractable>();
        grabbable.activated.AddListener(FireBullet);

        
        grabbable.activated.AddListener(StartRifleShoot); // Set listener for trigger pressed for rifle
        grabbable.deactivated.AddListener(EndRifleShoot); // Set listener for trigger released for rifle
    }

    // Update is called once per frame
    void Update()
    {


        if (shootingRifle && gameObject.tag == "Rifle") // If shooting rifle = true, gameObject's tag running the script == rifle
        {
            muzzleFlash.Play();
            GameObject spawnedBullet = Instantiate(bullet);
            spawnedBullet.transform.position = spawnPoint.position;
            spawnedBullet.GetComponent<Rigidbody>().velocity = spawnPoint.forward * fireSpeed;
            Destroy(spawnedBullet, 5);
        }
    }

    public void FireBullet(ActivateEventArgs arg)
    {
        // Run only if gameObject is pistol
        if (gameObject.tag == "Pistol")
        {
            // Play particle effect
            muzzleFlash.Play();
            GameObject spawnedBullet = Instantiate(bullet);
            spawnedBullet.transform.position = spawnPoint.position;
            spawnedBullet.GetComponent<Rigidbody>().velocity = spawnPoint.forward * fireSpeed;
            Destroy(spawnedBullet, 5);
        }
    }

    public void StartRifleShoot(ActivateEventArgs arg)
    {
        shootingRifle = true;
    }

    public void EndRifleShoot(DeactivateEventArgs arg)
    {
        shootingRifle = false;
    }

}

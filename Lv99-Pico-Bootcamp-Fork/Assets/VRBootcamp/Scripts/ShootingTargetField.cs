using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShootingTargetField : MonoBehaviour
{
    enum TargetMovement
    {
        Stationary, // Targets dont move
        HorizontalX, // Targets Move along the X axis
        HorizontalZ // Targets Move along the Z axis
    }

    enum FieldMode
    {
        InstantaneousContinuous,
        DelayedContinuous,
        Wave
    }

    enum FacingDirection
    {
        X,
        Z
    }

    [SerializeField] int NumberOfTargets = 1;
    [SerializeField] TargetMovement targetMovement;
    [SerializeField] float Speed = 1;
    [SerializeField] FieldMode fieldMode;
    [SerializeField] FacingDirection facingDirection;
    [SerializeField] TargetUI UI;
    [SerializeField] GameObject TargetPrefab;

    List<float> RespawnTime = new List<float>();
    List<Target> TargetList = new List<Target>();

    int CurrentScore;

    private void Awake()
    {
        for (int i = 0; i < NumberOfTargets; i++)
        {
            Target target = Instantiate(TargetPrefab, transform).GetComponent<Target>();

            if (facingDirection == FacingDirection.Z)
                target.transform.localScale = new Vector3(target.transform.localScale.x / transform.localScale.x,
                    target.transform.localScale.y / transform.localScale.y,
                    target.transform.localScale.z / transform.localScale.z);
            else
                target.transform.localScale = new Vector3(target.transform.localScale.x / transform.localScale.x / 2,
                    target.transform.localScale.y / transform.localScale.y,
                    target.transform.localScale.z / transform.localScale.z * 2);
            target.gameObject.SetActive(false);

            TargetList.Add(target);
        }

        SpawnAll();
    }

    void Spawn()
    {
        Target target = GetTarget(); // Get unused target
        RandomSpawn(target.transform); // randomise spawn location
    }

    void SpawnAll()
    {
        for (int i = 0; i < TargetList.Count; i++)
        {
            Target target = TargetList[i];
            if (!target.isActiveAndEnabled)
            {
                target.gameObject.SetActive(true);
                RandomSpawn(target.transform);
                continue;
            }
            RandomSpawn(target.transform);
        }
    }

    void ClearAll()
    {
        for (int i = 0; i < TargetList.Count; i++)
        {
            Target target = TargetList[i];
            target.gameObject.SetActive(false);
        }
    }

    private void Update()
    {
        Movement();
        RespawnTargets();
    }

    public void AddScore()
    {
        CurrentScore++;
        UI.UpdateScore(CurrentScore);
    }

    public void ClearScore()
    {
        CurrentScore = 0;
        UI.UpdateScore(CurrentScore);
    }

    void RandomSpawn(Transform trans)
    {
        trans.position = new Vector3(
                                 /*X:*/ Random.Range(transform.position.x - transform.localScale.x / 2 + trans.localScale.x / 2, transform.position.x + transform.localScale.x / 2 - trans.localScale.x / 2),
                                 /*Y:*/ transform.position.y,
                                 /*Z:*/ Random.Range(transform.position.z - transform.localScale.z / 2 + trans.localScale.z / 2, transform.position.z + transform.localScale.z / 2 - trans.localScale.z / 2));
    }

    void RespawnTargets()
    {
        switch(fieldMode)
        {
            case FieldMode.InstantaneousContinuous:
            case FieldMode.DelayedContinuous:
            case FieldMode.Wave:
                // check if still have anymore?
                if (CheckCleared())
                    SpawnAll();
                break;
            default: break;
        }
        for (int i = 0; i < RespawnTime.Count; i++)
        {
            RespawnTime[i] -= Time.deltaTime;
            if (RespawnTime[i] <= 0)
            {
                Spawn();
                RespawnTime.RemoveAt(i);
                i--;
            }

        }
    }

    bool CheckCleared()
    {
        for (int i = 0; i < TargetList.Count; i++)
        {
            if (TargetList[i].isActiveAndEnabled)
                return false;
        }
        return true;
    }

    Target GetTarget()
    {
        for (int i = 0; i < TargetList.Count; i++)
        {
            if (!TargetList[i].isActiveAndEnabled)
            {
                TargetList[i].gameObject.SetActive(true);
                return TargetList[i];
            }
        }
        return null;
    }

    void Movement()
    {
        switch(targetMovement)
        {
            case TargetMovement.Stationary: break;
            case TargetMovement.HorizontalX:
                HorizontalMovement(true);
                break;
            case TargetMovement.HorizontalZ:
                HorizontalMovement(false);
                break;
            default: break;
        }
    }

    void HorizontalMovement(bool direction)
    {
        for (int i = 0; i < TargetList.Count; i++)
        {
            Target target = TargetList[i];
            if (!target.gameObject.activeSelf)
                continue;

            if (direction)
                target.transform.position += new Vector3(Time.deltaTime * Speed * target.HorizontalDirection, 0, 0);
            else
                target.transform.position += new Vector3(0, 0, Time.deltaTime * Speed * target.HorizontalDirection);

            Bounce(target);
        }
    }

    void Bounce(Target target)
    {
        switch(targetMovement)
        {
            case TargetMovement.Stationary: break;
            case TargetMovement.HorizontalX:
                if (target.transform.position.x - target.transform.localScale.x / 2 < transform.position.x - transform.localScale.x / 2)
                    target.transform.position.Set(transform.position.x - transform.localScale.x / 2 + target.transform.localScale.x / 2, target.transform.position.y, target.transform.position.z);
                else if (target.transform.position.x + target.transform.localScale.x / 2 > transform.position.x + transform.localScale.x / 2)
                    target.transform.position.Set(transform.position.x + transform.localScale.x / 2 - target.transform.localScale.x / 2, target.transform.position.y, target.transform.position.z);
                else
                    break;
                target.HorizontalDirection *= -1;
                break;
            case TargetMovement.HorizontalZ:
                if (target.transform.position.z - target.transform.localScale.z / 2 < transform.position.z - transform.localScale.z / 2)
                    target.transform.position = new Vector3(target.transform.position.x, target.transform.position.y, transform.position.z - transform.localScale.z / 2 + target.transform.localScale.z / 2);
                else if (target.transform.position.z + target.transform.localScale.z / 2 > transform.position.z + transform.localScale.z / 2)
                    target.transform.position = new Vector3(target.transform.position.x, target.transform.position.y, transform.position.z + transform.localScale.z / 2 - target.transform.localScale.z / 2);
                else 
                    break;
                target.HorizontalDirection *= -1;
                break;
            default: break;
        }
    }
}

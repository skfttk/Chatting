using NUnit.Framework;
using System.Collections.Generic;
using TMPro;
using UnityEditor.PackageManager;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.UI;
using static UnityEngine.Rendering.DebugUI;

public class ChatHandler : MonoBehaviour
{
    TCPClient m_Client;
    private int m_iPort = 54000;
    private string m_sIp = "127.0.0.1";

    public TMP_InputField m_InputField;
    public RectTransform m_ContenRect;
    public GameObject m_TextPrefab;

    private bool m_IsEnter = false;
    private Queue<GameObject> m_ChatBoxList = new Queue<GameObject>();
    private int m_iPoolSize = 30;


    void Start()
    {
        m_Client = new TCPClient();
        m_Client.Connect(m_sIp, m_iPort);
        m_InputField.gameObject.SetActive(false);
        Create_Pool();
    }

    void Update()
    {
        if (Keyboard.current.enterKey.wasPressedThisFrame)
        {
            if (!m_IsEnter)
            {
                ChattingOn();
            }
            else
            {
                SnedMessageToServer();
            }
        }

        if (Keyboard.current.escapeKey.wasPressedThisFrame)
            Force_Close();

        if (m_Client != null && m_Client.HasData())
        {
            string msg = m_Client.Receive();
            AddChat(msg);
        }
    }

    private void ChattingOn()
    {
        m_IsEnter = true;
        m_InputField.gameObject.SetActive(m_IsEnter);
        m_InputField.Select();
        m_InputField.ActivateInputField();
    }

    private void SnedMessageToServer()
    {
        string text = m_InputField.text;
        m_Client.send(text);
        m_InputField.text = "";
        m_IsEnter = false;
        m_InputField.gameObject.SetActive(false);
    }

    private void AddChat(string msg)
    {
        GameObject obj = GetPool();
        TMP_Text text = obj.GetComponent<TMP_Text>();
        text.text = msg;
        obj.transform.SetParent(m_ContenRect, false);
    }

    private void Force_Close()
    {
        m_InputField.text = "";
        m_IsEnter = false;
        m_InputField.gameObject.SetActive(false);
    }

    private void Create_Pool()
    {
        for (int i = 0; i < m_iPoolSize; i++)
        {
            GameObject obj = Instantiate(m_TextPrefab);
            obj.SetActive(false);
            m_ChatBoxList.Enqueue(obj);
        }
    }

    private GameObject GetPool()
    {
        if (m_ChatBoxList.Count > 0)
        {
            var obj = m_ChatBoxList.Dequeue();
            obj.SetActive(true);
            return obj;
        }

        return Instantiate(m_TextPrefab);
    }
}

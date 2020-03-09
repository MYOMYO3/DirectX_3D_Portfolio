#pragma once
  
// HPTR�� �������� �����Ÿ� 
// �� ��ӹ�������?
class Ref
{
// ������� �߾���
// ������ ��ü����� ĸ��ȭ�� �������ټ� �ִ�.
private:
	// ����ð��� �þ�� ���̴�.
	// ���̷��� ���ø� 
	template<typename T>
	friend class HPTR;

private:
	int REFCOUNT;

	// �ߴ���.
	// �޸𸮴������� �Ҷ�.
// #ifdef DEBUGMODE
private:
	bool m_bDebug;

public:
	inline bool IsDebugCheck() 
	{
		return m_bDebug;
	}

	inline void DebugOn() {
		m_bDebug = true;
	}

	void DebugCheck();
// #endif

private:
	// ���Ѵ�
	// ���ȳ��� �ٽ� ����.
	void HADDREF();
	void HDECREF();

public:
	Ref();

	// �����Լ� ���̺�
	// 4����Ʈ.
	// �����Լ� ���̺� �翬�� ������.
	// ��ü������ �ٽ����� ��Ҷ�.
	// �ΰ����� �ڵ带 ĥ���� ����.
	// �޸� ������.
	// ��ĳ���� �Ǿ�����.
	// �ڽ� �Ҹ��ڰ� ȣ���� �ȵɼ��� �ִ�.
	// �Ҹ��� virtual 

protected:
	virtual ~Ref();
};

template<typename P>
class HPTR 
{
	// �������� ����� ����� ����� ���� �ִ�.
	// 

public:
	P* PTR;

public:
	//// ���� PARENT�� �ڽ��ΰ�?
	//template<typename PARENT> 
	//bool IsChild()	{	}

	//// ���� CHILD �� �θ��ΰ�????
	//template<typename CHILD>
	//bool IsParent() {	}

	operator P*()
	{
		return PTR;
	}


	bool operator <(const std::nullptr_t _PTR) const
	{
		return PTR < _PTR;
	}
	bool operator >(const std::nullptr_t _PTR) const
	{
		return PTR > _PTR;
	}

	// �ٸ� PTR
	bool operator <(const HPTR& _PTR) const
	{
		return PTR < _PTR.PTR;
	}
	bool operator >(const HPTR& _PTR) const
	{
		return PTR > _PTR.PTR;
	}

	// �ٸ� PTR
	bool operator <(P* const _PTR) const
	{
		return PTR < _PTR;
	}
	bool operator >(P* const _PTR) const
	{
		return PTR > _PTR;
	}

	// 1000 int
	// nullptr
	bool operator !=(const std::nullptr_t _PTR) const
	{
		return PTR != _PTR;
	}
	bool operator ==(const std::nullptr_t _PTR) const
	{
		return PTR == _PTR;
	}

// �ٸ� PTR
	bool operator !=(const HPTR& _PTR) const
	{
		return PTR != _PTR.PTR;
	}
	bool operator ==(const HPTR& _PTR) const
	{
		return PTR == _PTR.PTR;
	}

// �ٸ� PTR
	bool operator !=(P* const _PTR) const
	{
		return PTR != _PTR;
	}
	bool operator ==(P* const _PTR) const
	{
		return PTR == _PTR;
	}

// ->������
	P* const operator->() const
	{
		return PTR;
	}

	P& operator*() const
	{
		return (*PTR);
	}

// ���Կ����ڵ�.
	HPTR& operator =(P* const _PTR)
	{
		CheckDEC();
		PTR = _PTR;
		CheckADD();
		return *this;
	}

	HPTR& operator =(const std::nullptr_t _PTR)
	{
		CheckDEC();
		PTR = nullptr;
		return *this;
	}

	HPTR& operator =(const HPTR& _PTR)
	{
		// ���� ���� ����ִ� PTR�� �������� 
		// PTR
		// ���� ���� ����ִ� �༮�� �������� �����Ѵ�.
		CheckDEC();
		PTR = _PTR.PTR;
		// ���� ���Ӱ� ��Ե� �༮�� �������� �����Ѵ�.
		CheckADD();
		return *this;
	}

public:
	void CheckADD() 
	{
		if (nullptr == PTR)
		{
			return;
		}

		((Ref*)PTR)->HADDREF();
	}

	void CheckDEC()
	{
		if (nullptr == PTR)
		{
			return;
		}

		((Ref*)PTR)->HDECREF();
	}

	template<typename CON>
	CON* Convert() 
	{
		return dynamic_cast<CON*>(PTR);
	}

public:
	HPTR() : PTR(nullptr) {}
	HPTR(const std::nullptr_t _PTR) : PTR(_PTR) {}
	HPTR(P* const _PTR) : PTR(_PTR) 
	{
		CheckADD();
	}

	HPTR(const HPTR& _PTR) : PTR(_PTR.PTR)
	{
		CheckADD();
	}

public:
	virtual ~HPTR()
	{
		CheckDEC();
	}
};

// ���� ������ �����ε�
template<typename P>
typename bool operator==(const std::nullptr_t _PTR, const HPTR<P>& _HPTR) 
{
	return _PTR == _HPTR.PTR;
}
template<typename P>
typename bool operator!=(const std::nullptr_t _PTR, const HPTR<P>& _HPTR)
{
	return _PTR != _HPTR.PTR;
}

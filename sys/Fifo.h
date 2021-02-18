#ifndef FIFO_H
#define FIFO_H

template<typename T, unsigned int ENTRIES>
class Fifo
{
public:
  // Local Classes
  // Constructors
  Fifo();
  // Mutators: non-const operations
  bool push_back(const T& val); // producer
  bool fill(unsigned int fillsz, const T& val); // producer
  void pop_front(); // consumer
  void clear(); // consumer
  T& operator[] (unsigned int); // consumer
  // Accessors: const operations
  T& front() const; // consumer
  bool empty() const; // either
  size_t size() const; // either
  // Static and friend functions
  virtual ~Fifo();
  Fifo(const Fifo& object);
  Fifo& operator=(const Fifo& rhs)
  {
    if (this != &rhs)
    {
      memcpy(m_buf, rhs.m_buf, sizeof(T)*(ENTRIES+1));
      m_beg = m_buf;
      m_end = &m_buf[ENTRIES];

      m_pHead = (m_buf + (rhs.m_pHead - rhs.m_buf));
      m_pTail = (m_buf + (rhs.m_pTail - rhs.m_buf));
    }
    return(*this);
  }
private:
  // Data fields
  T m_buf[ENTRIES+1]; // Leave room for the empty data point
  T* m_beg;
  T* m_end;
  T* m_pHead;
  T* m_pTail;
};

/*------------------------------------------------------------------------------
struct GyroAccel
{
  int pitch;
  int yaw;
  float angle_acc;
  float angle_gyro;
} gas;
Constructor: FifoBuffer<struct GyroAccel, MAX_GAS> m_angles_fifo;
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline Fifo<T, ENTRIES>::Fifo()
  : m_beg(m_buf),
    m_end(&m_buf[ENTRIES]),
    m_pHead(m_buf),
    m_pTail(m_buf)
{
}

template<typename T, unsigned int ENTRIES>
inline Fifo<T, ENTRIES>::~Fifo()
{
}

/*------------------------------------------------------------------------------
FUNCTION:      bool push_back(const T& val);
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline bool Fifo<T, ENTRIES>::push_back(const T& val)
{
  bool retVal = false;

  T* pHead = m_pHead;

  // Check for wrap and wrap as needed.
  if (++pHead > m_end)
  {
    pHead = m_beg;
  }

  // If the head does not == the tail we have not overflowed
  if (pHead != m_pTail)
  {
    *m_pHead = val;  // Now save the value
    m_pHead = pHead; // Now go to the next location.
    retVal = true;
  }

  return(retVal);
}

/*------------------------------------------------------------------------------
FUNCTION:      bool fill(unsigned int fillsz, const T& val);
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline bool Fifo<T, ENTRIES>::fill(unsigned int fillsz, const T& val)
{
  bool retVal = false;

  // Handle the case where the buffer can already be partially filled.
  int bufsz = m_pHead - m_pTail;
  if (bufsz<0)
  {
    // So adjust the size for the wrap case
    bufsz += (ENTRIES+1);
  }

  // Check if this buffer is large enough for the fill
  if (fillsz <= (ENTRIES-bufsz))
  {
    T* phead = m_pHead;

    // Fill and handle wrap
    while(fillsz--)
    {
      *phead++ = val;

      // If we hit the end of the buffer just wrap.
      if (phead > m_end)
      {
        phead = m_beg;
      }
    }

    m_pHead = phead; // Now move head after we are done filling.

    retVal = true; // The fill was sucessful
  }

  return(retVal);
}

/*------------------------------------------------------------------------------
FUNCTION:      T front()
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline T& Fifo<T, ENTRIES>::front() const
{
  return(*m_pTail);
}

/*------------------------------------------------------------------------------
FUNCTION:      void pop_front()
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline void Fifo<T, ENTRIES>::pop_front()
{
  T* pTail = m_pTail;

  // Check if the buffer has some data.
  if (pTail != m_pHead)
  {
    // Check for wrap and wrap as needed
    if (++pTail > m_end)
    {
      pTail = m_beg;
    }

    // Now move the pointer which removes the data point
    m_pTail = pTail;
  }

  return;
}

/*------------------------------------------------------------------------------
FUNCTION:      void clear()
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline void Fifo<T, ENTRIES>::clear()
{
  // Set the tail to the head to make the buffer empty.
  m_pTail = m_pHead;

  return;
}

/*------------------------------------------------------------------------------
FUNCTION:      bool empty() const
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline bool Fifo<T, ENTRIES>::empty() const
{
  return(m_pTail == m_pHead);
}

/*------------------------------------------------------------------------------
FUNCTION:      size_t size() const
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline size_t Fifo<T, ENTRIES>::size() const
{
  size_t size = 0;

  // Get snapshots of head/tail so that the values used in the comparison will
  // be the same values used (and assumed) by the selected size calculation.
  T* pTail = m_pTail;
  T* pHead = m_pHead;

  // If head and tail cross the bounds of the buffer
  if(pHead < pTail)
  {
    // So adjust the size for the wrap case.
    // Using MAX_ELEMENTS+1 since that is the actual buffer size
    size = (ENTRIES + 1) - (pTail - pHead);
  }
  else
  {
    size = pHead - pTail;
  }

  return size;
}

/*------------------------------------------------------------------------------
FUNCTION:      T& operator[] (unsigned int index)
------------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline T& Fifo<T, ENTRIES>::operator[] (unsigned int index)
{
  T* ptail = m_pTail;

  // Get the buffer size
  unsigned int bufsize = size();

  // Check if this is a valid index. This also takes care of bufsize == 0, buffer empty.
  if (bufsize > index)
  {
    ptail += index;

    // If this would wrap
    if (ptail > m_end)
    {
      // start from the beginning and add up the amount ptail passes the end,
      // this does assume we are not at the very end of memory to start.
      ptail = (m_beg + (ptail - m_end)) - 1;
    }
  }

  return(*ptail);
}

/*----------------------------------------------------------------------
FUNCTION:    Copy constructor
------------------------------------------------------------------------*/
template<typename T, unsigned int ENTRIES>
inline Fifo<T, ENTRIES>::Fifo(const Fifo& other)
: m_beg(m_buf),
  m_end(&m_buf[ENTRIES]),
  m_pHead(m_buf + (other.m_pHead - other.m_buf)),
  m_pTail(m_buf + (other.m_pTail - other.m_buf))
{
  // Copy the fifo buffer data from other
  memcpy(m_buf, other.m_buf, sizeof(T)*(ENTRIES+1));
}

#endif /* FIFO_H */

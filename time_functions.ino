inline int getMins(long t)
{
  return t / 1000 / 60 % 60;
}
inline int getHours(long t)
{
  return t / 1000 / 3600 % 24;
}
inline int getSecs(long t)
{
  return t / 1000 % 60;
}
inline bool blinking(int del)
{
  if (del >= MAX_OFFSET)
    return false;
  return del <= 0 || currentTime % (del * 2) <= del;
}


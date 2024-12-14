namespace aoc2015.days;

public class Day20 : IDay
{
    public uint Day => 20;

    // too naive
    private static int Dividers(int no)
    {
        var sum = 0;

        for (var i = 1; i <= no; i++)
        {
            if (no % i == 0)
                sum += (i*10);
        }

        return sum;
    }


    private static int Factors(int no)
    {
        var fsum = 0;
        var f = 1;
        var sq = (int) Math.Sqrt(no);
        while (f <= sq)
        {
            if (no % f == 0)
            {
                fsum += (f * 10);

                if(f != (no/f))
                    fsum += ((no / f)) * 10;
            }
            f++;
        }

        return fsum;
    }

    private static int FactorsLazyElves(int no)
    {
        var fsum = 0;
        var f = 1;
        var sq = (int) Math.Sqrt(no);
        while (f <= sq)
        {
            if (no % f == 0)
            {
                if(no/f <= 50)
                    fsum += (f * 11);

                if (f != (no / f))
                    if (f <= 50)
                        fsum += ((no / f)) * 11;
            }

            f++;
        }

        return fsum;
    }

    public void Run()
    {

        // House 12:

        // 120 + 40 + 30 + 20 + 10

        const int upto = int.MaxValue;

        for (var i = 1; i < upto; i++)
        {
            var presents = FactorsLazyElves(i);
            // Console.WriteLine($"{i}: {presents}");
            if (presents >= 34000000)
            {
                Console.WriteLine($"Reached present threshold after passing the {i}'th house");
                break;
            }

        }

    }
}

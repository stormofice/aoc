namespace aoc2015.days;

public class Day17 : IDay
{
    public uint Day => 17;

    public void Run()
    {
        // List<int> sizes = [20, 15, 10, 5, 5];
        var sizes = Utils.GetDayInput(this)
            .Split("\n", StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries).Select(int.Parse)
            .ToList();
        Console.WriteLine($"Sizes: [{string.Join(",", sizes)}] (#{sizes.Count})");
        var combs = 0;

        var total = (1 << sizes.Count) - 1;
        Console.WriteLine($"Total Combinations: {total}");

        var isBitSet = (long num, int idx) => ((num >> idx) & 1) == 1;

        int[] sizeCount = new int[sizes.Count];

        for (var i = 0; i < total; i++)
        {
            List<int> currentAttempt = [];

            for (var k = 0; k < sizes.Count; k++)
            {
                if (isBitSet(i, k))
                {
                    currentAttempt.Add(sizes[k]);
                }
            }

            var sum = currentAttempt.Sum();
            if (sum == 150)
            {
                sizeCount[currentAttempt.Count]++;
                Console.WriteLine($"[{string.Join(",", currentAttempt)}]: {sum}");
                combs++;
            }
        }

        Console.WriteLine($"{combs} combinations can fill the container perfectly");

        for (var i = 0; i < sizeCount.Length; i++)
        {
            Console.WriteLine($"{i}: {sizeCount[i]}");
        }
    }
}

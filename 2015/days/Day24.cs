namespace aoc2015.days;

public class Day24 : IDay
{
    public uint Day => 24;


    private static IEnumerable<T[]> GetCombinations<T>(T[] input, int k)
    {
        if (k == 0)
            yield break;

        var indices = new int[k];

        for (var i = 0; i < indices.Length; i++)
        {
            indices[i] = i;
        }

        while (true)
        {
            var current = new T[k];
            for (var i = 0; i < current.Length; i++)
            {
                current[i] = input[indices[i]];
            }

            yield return current;


            var right = k - 1;
            while (right >= 0 && indices[right] == input.Length - (k - right))
            {
                right--;
            }

            if (right < 0)
                yield break;

            indices[right]++;
            for (var i = right + 1; i < k; i++)
            {
                indices[i] = indices[i - 1] + 1;
            }
        }
    }

    private static bool CanSum(ulong[] numbers, ulong target)
    {
        var dp = new bool[numbers.Length + 1, target + 1];
        for (var i = 0; i <= numbers.Length; i++)
        {
            dp[i, 0] = true;
        }

        for (var i = 1; i <= numbers.Length; i++)
        {
            var currentNo = numbers[i - 1];

            for (ulong sum = 0; sum <= target; sum++)
            {
                dp[i, sum] = dp[i - 1, sum];

                if (sum >= currentNo)
                {
                    // var canMakeSelf = dp[i, sum];
                    var canMakeRest = dp[i - 1, sum - currentNo];
                    dp[i, sum] |= canMakeRest;
                }
            }
        }

        return dp[numbers.Length, target];
    }

    public record SumRes
    {
        public bool CanSum { get; init; }
        public HashSet<ulong> UsedNos { get; init; } = [];
    }

    public static SumRes TrackMakeSum(ulong[] numbers, ulong targetSum)
    {
        var dp = new SumRes[numbers.Length + 1, targetSum + 1];

        for (var i = 0; i <= numbers.Length; i++)
        {
            for (ulong j = 0; j <= targetSum; j++)
            {
                dp[i, j] = new() {CanSum = false};
            }
        }

        for (var i = 0; i <= numbers.Length; i++)
            dp[i, 0] = new() {CanSum = true};

        for (var i = 1; i <= numbers.Length; i++)
        {
            var currentNo = numbers[i - 1];

            for (ulong sum = 0; sum <= targetSum; sum++)
            {
                if (dp[i - 1, sum].CanSum)
                {
                    dp[i, sum] = new()
                    {
                        CanSum = true,
                        UsedNos = [..dp[i - 1, sum].UsedNos],
                    };
                }

                if (sum >= currentNo)
                {
                    var remSum = sum - currentNo;
                    if (dp[i - 1, remSum].CanSum)
                    {
                        var prevNoUs = new HashSet<ulong>(dp[i - 1, remSum].UsedNos) {currentNo};

                        if (!dp[i, sum].CanSum ||
                            prevNoUs.Count < dp[i, sum].UsedNos.Count)
                        {
                            dp[i, sum] = new()
                            {
                                CanSum = true,
                                UsedNos = prevNoUs,
                            };
                        }
                    }
                }
            }
        }

        return dp[numbers.Length, targetSum];
    }

    public void Run()
    {
        var packages = Utils.GetDayInput(this).Split("\n").Select(ulong.Parse).ToArray();

        ulong totalWeight = packages.Aggregate(0UL, (acc, v) => acc + v);
        ulong perGroupWeight = totalWeight / 4;
        Console.WriteLine($"Total: {totalWeight}, PerGroup: {perGroupWeight}");

        var lowestQe = ulong.MaxValue;

        var hit = false;

        // search for potential first groups
        for (var gs = 2; gs < packages.Length - 2; gs++)
        {
            var combs = GetCombinations(packages, gs);

            foreach (var comb in combs)
            {
                var fgSum = comb.Aggregate(0UL, (acc, v) => acc + v);
                if (fgSum == perGroupWeight)
                {
                    Console.WriteLine($"Potential FG: {string.Join(",", comb)}");

                    var ex = packages.Except(comb).ToArray();

                    var track = TrackMakeSum(ex, perGroupWeight);

                    if (track.CanSum)
                    {
                        var rems = ex.Except(track.UsedNos).ToArray();
                        if (CanSum(rems, perGroupWeight))
                        {
                            lowestQe = Math.Min(lowestQe, comb.Aggregate(1UL, (acc, v) => acc * v));
                            hit = true;
                        }
                    }
                }
            }

            if (hit)
                break;
        }

        Console.WriteLine($"Lowest QE: {lowestQe}");
    }

    public void RunPt1Dp()
    {
        // ulong[] packages = [1, 2, 3, 4, 5, 7, 8, 9, 10, 11];
        var packages = Utils.GetDayInput(this).Split("\n").Select(ulong.Parse).ToArray();

        ulong totalWeight = packages.Aggregate(0UL, (acc, v) => acc + v);
        ulong perGroupWeight = totalWeight / 3;
        Console.WriteLine($"Total: {totalWeight}, PerGroup: {perGroupWeight}");

        var lowestQe = ulong.MaxValue;

        var hit = false;

        // search for potential first groups
        for (var gs = 2; gs < packages.Length - 2; gs++)
        {
            var combs = GetCombinations(packages, gs);

            foreach (var comb in combs)
            {
                var fgSum = comb.Aggregate(0UL, (acc, v) => acc + v);
                if (fgSum == perGroupWeight)
                {
                    Console.WriteLine($"Potential FG: {string.Join(",", comb)}");

                    var ex = packages.Except(comb).ToArray();

                    if (CanSum(ex, perGroupWeight))
                    {
                        lowestQe = Math.Min(lowestQe, comb.Aggregate(1UL, (acc, v) => acc * v));
                        hit = true;
                    }
                }
            }

            if (hit)
                break;
        }

        Console.WriteLine($"Lowest QE: {lowestQe}");
    }

    public void RunPt1()
    {
        // ulong[] packages = [1, 2, 3, 4, 5, 7, 8, 9, 10, 11];
        var packages = Utils.GetDayInput(this).Split("\n").Select(ulong.Parse).ToArray();

        ulong totalWeight = packages.Aggregate(0UL, (acc, v) => acc + v);
        ulong perGroupWeight = totalWeight / 3;
        Console.WriteLine($"Total: {totalWeight}, PerGroup: {perGroupWeight}");

        var lowestQe = ulong.MaxValue;

        var hit = false;

        // search for potential first groups
        for (var gs = 2; gs < packages.Length - 2; gs++)
        {
            var combs = GetCombinations(packages, gs);

            foreach (var comb in combs)
            {
                var fgSum = comb.Aggregate(0UL, (acc, v) => acc + v);
                if (fgSum == perGroupWeight)
                {
                    Console.WriteLine($"Potential FG: {string.Join(",", comb)}");

                    var remaining = packages.Where(p => !comb.Contains(p)).ToList();

                    // check if we can split into two with target weight

                    for (var sgs = 1; sgs < remaining.Count; sgs++)
                    {
                        foreach (var sg in GetCombinations(remaining.ToArray(), sgs))
                        {
                            if (sg.Aggregate(0UL, (acc, v) => acc + v) == perGroupWeight)
                            {
                                var ex = remaining.Except(sg);
                                if (ex.Aggregate(0UL, (acc, v) => acc + v) == perGroupWeight)
                                {
                                    lowestQe = Math.Min(lowestQe, comb.Aggregate(1UL, (acc, v) => acc * v));
                                    hit = true;
                                }
                            }
                        }
                    }
                }
            }

            if (hit)
                break;
        }

        Console.WriteLine($"Lowest QE: {lowestQe}");
    }
}

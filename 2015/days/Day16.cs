using System.Diagnostics;

namespace aoc2015.days;

public class Day16 : IDay
{
    public uint Day => 16;

    private class Sue
    {
        internal int Children;
        internal int Cats;
        internal int Samoyeds;
        internal int Pomeranians;
        internal int Akitas;
        internal int Vizslas;
        internal int Goldfish;
        internal int Trees;
        internal int Cars;
        internal int Perfumes;

        public Sue(int children,
            int cats,
            int samoyeds,
            int pomeranians,
            int akitas,
            int vizslas,
            int goldfish,
            int trees,
            int cars,
            int perfumes)
        {
            Children = children;
            Cats = cats;
            Samoyeds = samoyeds;
            Pomeranians = pomeranians;
            Akitas = akitas;
            Vizslas = vizslas;
            Goldfish = goldfish;
            Trees = trees;
            Cars = cars;
            Perfumes = perfumes;
        }

        public override bool Equals(object? obj)
        {
            if (obj is not Sue other) return false;
            var eqOrZero = (int a, int b) => a == b || a == -1 || b == -1;
            // pt2
            var largerOrZero = (int a, int b) => a > b || a == -1 || b == -1;
            var smallerOrZero = (int a, int b) => a < b || a == -1 || b == -1;

            return eqOrZero(this.Children, other.Children)
                   && largerOrZero(this.Cats, other.Cats)
                   && eqOrZero(this.Samoyeds, other.Samoyeds)
                   && smallerOrZero(this.Pomeranians, other.Pomeranians)
                   && eqOrZero(this.Akitas, other.Akitas)
                   && eqOrZero(this.Vizslas, other.Vizslas)
                   && smallerOrZero(this.Goldfish, other.Goldfish)
                   && largerOrZero(this.Trees, other.Trees)
                   && eqOrZero(this.Cars, other.Cars)
                   && eqOrZero(this.Perfumes, other.Perfumes);

        }
    }

    public void Run()
    {
        var inp = Utils.GetDayInput(this);

        var target = new Sue(3, 7, 2, 3, 0, 0, 5, 3, 2, 1);

        var idx = 1;
        foreach (var line in inp.Split("\n"))
        {
            var cs = line.Split(":", 2)[1];
            var com = cs.Split(",");
            Trace.Assert(com.Length == 3);

            Sue current = new Sue(-1,-1, -1, -1, -1, -1, -1, -1, -1, -1);
            foreach (var se in com)
            {
                var ts = se.Split(":").Select(s => s.Trim()).ToList();
                var v = int.Parse(ts[1]);
                switch (ts[0])
                {
                    case "children":
                        current.Children = v;
                        break;
                    case "cats":
                        current.Cats = v;
                        break;
                    case "samoyeds":
                        current.Samoyeds = v;
                        break;
                    case "pomeranians":
                        current.Pomeranians = v;
                        break;
                    case "akitas":
                        current.Akitas = v;
                        break;
                    case "vizslas":
                        current.Vizslas = v;
                        break;
                    case "goldfish":
                        current.Goldfish = v;
                        break;
                    case "trees":
                        current.Trees = v;
                        break;
                    case "cars":
                        current.Cars = v;
                        break;
                    case "perfumes":
                        current.Perfumes = v;
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }

            if (current.Equals(target))
            {
                Console.WriteLine($"Sue {idx} is the target");
                break;
            }


            idx++;
        }
    }
}

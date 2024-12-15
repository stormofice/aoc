using System.Diagnostics;

namespace aoc2015.days;

public class Day21 : IDay
{
    private class Player
    {
        internal int Damage;
        internal int Armor;
        internal int Hitpoints;
        private readonly List<Item> Items = [];

        internal void Reset()
        {
            Damage = 0;
            Armor = 0;
            Hitpoints = 100;
            Items.Clear();
        }

        internal void AddItem(Item i)
        {
            Damage += i.Damage;
            Armor += i.Armor;
            Items.Add(i);
        }

        internal int Cost => Items.Sum(i => i.Cost);
    }

    private class Item
    {
        internal readonly int Cost;
        internal readonly int Damage;
        internal readonly int Armor;

        public Item(int cost, int damage, int armor)
        {
            Cost = cost;
            Damage = damage;
            Armor = armor;
        }

        public Item(int cost, int damage)
        {
            Cost = cost;
            Damage = damage;
        }
    }

    private class Boss
    {
        internal int Hitpoints = 104;
        internal readonly int Damage = 8;
        internal readonly int Armor = 1;

        internal void Reset()
        {
            Hitpoints = 104;
        }
    }

    public uint Day => 21;

    private int Simulations;

    public void Run()
    {
        var player = new Player();
        var boss = new Boss();

        List<Item> weapons =
        [
            new(8, 4),
            new(10, 5),
            new(25, 6),
            new(40, 7),
            new(75, 8),
        ];

        List<Item> armors =
        [
            new(13, 0, 1),
            new(31, 0, 2),
            new(53, 0, 3),
            new(75, 0, 4),
            new(102, 0, 5),
        ];

        List<Item> rings =
        [
            new(25, 1),
            new(50, 2),
            new(100, 3),
            new(20, 0, 1),
            new(40, 0, 2),
            new(80, 0, 3),
        ];

        var minCost = int.MaxValue;
        var maxCostToLose = int.MinValue;
        // hehe
        foreach (var weapon in weapons)
        {
            foreach (var armor in armors)
            {
                foreach (var ring1 in rings)
                {
                    foreach (var ring2 in rings)
                    {
                        player.AddItem(weapon);
                        player.AddItem(armor);
                        player.AddItem(ring1);
                        player.AddItem(ring2);
                        var res = Fight(player, boss);
                        if (res)
                        {
                            minCost = Math.Min(minCost, player.Cost);
                        }
                        else
                        {
                            maxCostToLose = Math.Max(maxCostToLose, player.Cost);
                        }

                        player.Reset();
                        boss.Reset();
                    }

                    player.AddItem(weapon);
                    player.AddItem(armor);
                    player.AddItem(ring1);
                    var oneringres = Fight(player, boss);
                    if (oneringres)
                    {
                        minCost = Math.Min(minCost, player.Cost);
                    }
                    else
                    {
                        maxCostToLose = Math.Max(maxCostToLose, player.Cost);
                    }

                    player.Reset();
                    boss.Reset();
                }
            }

            foreach (var ring1 in rings)
            {
                foreach (var ring2 in rings)
                {
                    player.AddItem(weapon);
                    player.AddItem(ring1);
                    player.AddItem(ring2);
                    var res = Fight(player, boss);
                    if (res)
                    {
                        minCost = Math.Min(minCost, player.Cost);
                    }
                    else
                    {
                        maxCostToLose = Math.Max(maxCostToLose, player.Cost);
                    }

                    player.Reset();
                    boss.Reset();
                }

                player.AddItem(weapon);
                player.AddItem(ring1);
                var oneringres = Fight(player, boss);
                if (oneringres)
                {
                    minCost = Math.Min(minCost, player.Cost);
                }
                else
                {
                    maxCostToLose = Math.Max(maxCostToLose, player.Cost);
                }

                player.Reset();
                boss.Reset();
            }
        }

        Console.WriteLine(
            $"Minimum cost to win: {minCost}, Maximum cost to lose: {maxCostToLose} (#{Simulations} simulations)");
    }

    private bool Fight(Player p, Boss b)
    {
        Simulations++;
        while (true)
        {
            b.Hitpoints -= (p.Damage - b.Armor);
            if (b.Hitpoints <= 0)
                return true;
            p.Hitpoints -= (b.Damage - p.Armor);
            if (p.Hitpoints <= 0)
                return false;
        }
    }
}

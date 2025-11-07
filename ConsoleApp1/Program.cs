using System;
using System.Net;

class SubnetCalculator
{
    static uint IpToUint(string ip)
    {
        return (uint)IPAddress.Parse(ip).Address;
    }
    static string UintToIp(uint ip)
    {
        return new IPAddress(ip).ToString();
    }

    static void Main(string[] args)
    {
        Console.Write("Введите IP-адрес: ");
        string ipStr = Console.ReadLine();
        Console.Write("Введите маску подсети (например, 255.255.255.0): ");
        string maskStr = Console.ReadLine();

        uint ip = IpToUint(ipStr);
        uint mask = IpToUint(maskStr);

        uint network = ip & mask;
        uint broadcast = network | ~mask;

        int hostBits = 32 - CountBits(mask);
        int totalIps = (int)Math.Pow(2, hostBits);
        int usableHosts = totalIps <= 2 ? totalIps : totalIps - 2;

        Console.WriteLine($"Адрес сети: {UintToIp(network)}");
        Console.WriteLine($"Широковещательный адрес: {UintToIp(broadcast)}");
        Console.WriteLine($"Количество IP-адресов: {totalIps}");
        Console.WriteLine($"Количество узлов: {usableHosts}");
    }
    static int CountBits(uint mask)
    {
        int count = 0;
        while (mask != 0)
        {
            count += (int)(mask & 1);
            mask >>= 1;
        }
        return count;
    }
}

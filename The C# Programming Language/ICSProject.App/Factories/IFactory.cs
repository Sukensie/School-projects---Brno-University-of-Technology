namespace ICSProject.App.Factories
{
    public interface IFactory<out T>
    {
        T Create();
    }
}
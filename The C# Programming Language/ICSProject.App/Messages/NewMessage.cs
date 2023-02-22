using ICSProject.BL.Models;

namespace ICSProject.App.Messages
{
    public record NewMessage<T> : Message<T> where T : IModel { }
}

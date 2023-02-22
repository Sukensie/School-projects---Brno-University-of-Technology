using ICSProject.BL.Models;

namespace ICSProject.App.Messages
{
    public record UpdateMessage<T> : Message<T>
        where T : IModel
    {
    }
}

using System.Windows;
using System.Windows.Input;

namespace ICSProject.App.Controls
{
    public partial class Label
    {
        public ICommand Cmd
        {
            get => (ICommand)GetValue(CmdProperty);
            set => SetValue(CmdProperty, value); 
        }

        public string Title
        {
            get => (string)GetValue(TitleProperty);
            set => SetValue(TitleProperty, value); 
        }

        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }


        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(Label),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title),
            typeof(string),
            typeof(Label),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty CmdProperty = DependencyProperty.Register(
            nameof(Cmd),
            typeof(ICommand),
            typeof(Label),
            new PropertyMetadata(default(ICommand)));

        public Label()
        {
            InitializeComponent();
        }
    }
}
